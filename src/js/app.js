var keys = require('message_keys');
// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);


var apiKey = '';
var temperatureUnit = 'metric';


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
    console.log('Requesting weather for lat=' + pos.coords.latitude + ' lon=' + pos.coords.longitude + 
        ' locale=de' + ' units=' + temperatureUnit);
    
    // Construct URL
    var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
        pos.coords.latitude + '&lon=' + pos.coords.longitude + 
        '&appid=' + apiKey +
        '&lang=de' +
        '&units=' + temperatureUnit;
  
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
      
      key = keys.UNIT_TEMP;
      var useCelsius = e.payload[key];
      if (typeof useCelsius === "undefined") {
        useCelsius = e.payload.UNIT_TEMP;
      }
      if (typeof useCelsius === "undefined") {
        console.log("No dice - useCelsius still undefined!");
      } else {
        temperatureUnit = useCelsius === 0 ? 'imperial' : 'metric';
        console.log('Accessing payload: ' + useCelsius + ' ' + (typeof useCelsius) + ' -->' + temperatureUnit);
      }
    }
    
    getWeather();
  }                     
);


Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var dict = {};
  dict.OWM_APPID = configData.api_key;
  
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});


