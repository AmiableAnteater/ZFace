var keys = require('message_keys');

var apiKey = '';



var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};



function locationSuccess(pos) {
  if (apiKey) {
    // Construct URL
    var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
        pos.coords.latitude + '&lon=' + pos.coords.longitude + 
        '&appid=' + apiKey +
        '&lang=de' +
        '&units=metric';
  
    // Send request to OpenWeatherMap
    xhrRequest(url, 'GET', 
      function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);
        console.log('OpenWeatherMap returned:\n' + JSON.stringify(json, null, 4));
        
        // Temperature in Kelvin requires adjustment
        var temperature = Math.round(json.main.temp);
        console.log('Temperature is ' + temperature);
  
        // Conditions
        var conditions = json.weather[0].main;      
        console.log('Conditions are ' + conditions);
        
        // Assemble dictionary using our keys
        var dictionary = {
          'TEMPERATURE': temperature,
          'CONDITIONS': conditions
        };
        
        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log('Weather info sent to Pebble successfully!');
          },
          function(e) {
            console.log('Error sending weather info to Pebble!');
          }
        );
              
      }      
    );
  } else {
    console.log('No API KEY!!!');
  }
}

/*
function locationSuccess(pos) {
  console.log('Got location - generating random weather.');
  var max = 45;
  var min = -15;
  var temperature = Math.floor(Math.random() * (max - min + 1)) + min;
  
  var currentdate = new Date(); 
  var conditions = currentdate.getHours() + ":" + currentdate.getMinutes() + ":" + currentdate.getSeconds();
  
  var dictionary = {
    'TEMPERATURE': temperature,
    'CONDITIONS': conditions
  };
      
  // Send to Pebble
  Pebble.sendAppMessage(dictionary,
                        function(e) {
                          console.log('Pseudo weather info sent to Pebble successfully!');
                        },
                        function(e) {
                          console.log('Error sending pseudo weather info to Pebble!');
                        }
                       );
}
*/

function locationError(err) {
  console.log('Error requesting location!');
}



function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 30000, maximumAge: 300000}
  );
}



// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    var dictionary = {
      'JS_KIT_READY': 1
    };
    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
                        function(e) {
                          console.log('Signaled PebbleKit JS readyness successfully!');
                        },
                        function(e) {
                          console.log('Error sending PebbleKit JS readyness!');
                        }
                       );
  }
);



// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    /*
    var skeys = JSON.stringify(keys, null, 4); 
    console.log('AppMessage received!' + skeys);
    var se = JSON.stringify(e, null, 4); 
    console.log('                 e= ' + se);
    */

    if (keys) {
      var key = keys.OWM_APPID;
      var appid = e.payload[key];
      if (typeof appid === "undefined") {
        appid = e.payload.OWM_APPID;
      }
      if (typeof appid === "undefined") {
        console.log("No dice - appid still undefined!");
      } else {
        console.log('Accessing payload: ' + appid + ' ' + (typeof appid) );
        apiKey = appid;
      }
    }
    
    getWeather();
  }                     
);



Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://azpebconfig.site88.net/index.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});



Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var backgroundColor = configData.background_color;

  var dict = {};
  if(configData.high_contrast === true) {
    dict.HIGH_CONTRAST = configData.high_contrast ? 1 : 0;  // Send a boolean as an integer
  } else {
    dict.COLOR_RED = parseInt(backgroundColor.substring(2, 4), 16);
    dict.COLOR_GREEN = parseInt(backgroundColor.substring(4, 6), 16);
    dict.COLOR_BLUE = parseInt(backgroundColor.substring(6), 16);
  }

  dict.OWM_APPID = configData.api_key;
  
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});