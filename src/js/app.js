var keys = require('message_keys');
// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });


var apiKey = '';
var temperatureUnit = 'metric';
var weatherLocale = 'en';
var LOCALE_DIC = {
  500:'bg',
  1000:'ca',
  1500:'zh',
  2000:'zh_tw',
  2500:'hr',
  3000:'nl',
  3500:'en',
  4000:'fi',
  4500:'fr',
  5000:'de',
  5500:'it',
  6000:'pl',
  6500:'pt',
  7000:'ro',
  7500:'ru',
  8000:'es',
  8500:'sv',
  9000:'tr',
  9500:'uk'
};




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
        '&lang=' + weatherLocale +
        '&units=' + temperatureUnit;
    
    console.log('Using URL: ' + url);
  
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
        var conditions = json.weather[0].description;      
        console.log('Conditions are ' + conditions);
        
        // Assemble dictionary using our keys
        var dictionary = {
          'TEMPERATURE': temperature,
          'CONDITION_DESC': conditions
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
        console.log("Access with key " + key + " failed. Trying OWM_APPID instead.");
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
        console.log("Access with key " + key + " failed. Trying UNIT_TEMP instead.");
        useCelsius = e.payload.UNIT_TEMP;
      }
      if (typeof useCelsius === "undefined") {
        console.log("No dice - useCelsius still undefined!");
      } else {
        temperatureUnit = useCelsius === 0 ? 'imperial' : 'metric';
        console.log('Accessing payload: ' + useCelsius + ' ' + (typeof useCelsius) + ' --> ' + temperatureUnit);
      }
      
      key = keys.WEATHER_LOCALE;
      var localeAsInt = e.payload[key];
      if (typeof localeAsInt === "undefined") {
        console.log("Access with key " + key + " failed. Trying WEATHER_LOCALE instead.");
        localeAsInt = e.payload.WEATHER_LOCALE;
      }
      if (typeof localeAsInt === "undefined") {
        console.log("No dice - localeAsInt still undefined!");
      } else {
        console.log('Access key for locale dict is ' + localeAsInt + ' (an' + (typeof localeAsInt) + ')');
        weatherLocale = LOCALE_DIC[localeAsInt] || 'en';
        console.log('Accessing payload: ' + LOCALE_DIC[localeAsInt] + ' --> using locale ' + weatherLocale);
      }      
    }
    
    getWeather();
  }                     
);



Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});



Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }

  // Get the keys and values from each config item
  var dict = clay.getSettings(e.response);
  if (dict[keys.WEATHER_LOCALE]) {
    dict[keys.WEATHER_LOCALE] = parseInt(dict[keys.WEATHER_LOCALE]);
    console.log('Converted locale!');
  }
  console.log('Config page returned:\n' + JSON.stringify(dict, null, 4));
  
  // Send settings values to watch side
  Pebble.sendAppMessage(dict, function(e) {
    console.log('Sent config data to Pebble');
  }, function(e) {
    console.log('Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});