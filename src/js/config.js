module.exports = [
  {
    "type": "heading",
    "defaultValue": "ZFace Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Dear user - thanks for using ZFace. To use the watchface effectively, you need to configure your OpenWeatherMap API key. All other settings are optional."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Weather"
      },
      {
        "type": "input",
        "messageKey": "OWM_APPID",
        "label": "OpenWeatherMap API key",
        "attributes": {
          "type": "text",
          "placeholder": "e.g. 6293a029546f36d3e7df246ae2e09bd2"
        }
      },
      {
        "type": "slider",
        "messageKey": "UPDATE_FREQ",
        "defaultValue": 10,
        "label": "Update frequency in minutes",
        "description": "Configure how often the current weather is updated.",
        "min": 5,
        "max": 120,
        "step": 5
      },
      {
        "type": "radiogroup",
        "messageKey": "UNIT_TEMP",
        "label": "Temperature Unit",
        "defaultValue": "C",
        "options": [
          { 
            "label": "°Celsius", 
            "value": "C" 
          },
          { 
            "label": "°Fahrenheit", 
            "value": "F" 
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "WEATHER_LOCALE",
        "defaultValue": "3500",
        "label": "Locale (for weather)",
        "options": [
          { 
            "label": "Bulgarian", 
            "value": "500" 
          },
          { 
            "label": "Catalan",
            "value": "1000" 
          },
          { 
            "label": "Chinese (simpl.)",
            "value": "1500" 
          },
          { 
            "label": "Chinese (trad.)",
            "value": "2000" 
          },
          { 
            "label": "Croatian",
            "value": "2500" 
          },
          { 
            "label": "Dutch",
            "value": "3000" 
          },
          { 
            "label": "English",
            "value": "3500" 
          },
          { 
            "label": "Finnish",
            "value": "4000" 
          },
          { 
            "label": "French",
            "value": "4500" 
          },
          { 
            "label": "German",
            "value": "5000" 
          },
          { 
            "label": "Italian",
            "value": "5500" 
          },
          { 
            "label": "Polish",
            "value": "6000" 
          },
          { 
            "label": "Portuguese",
            "value": "6500" 
          },
          { 
            "label": "Romanian",
            "value": "7000" 
          },
          { 
            "label": "Russian",
            "value": "7500" 
          },
          { 
            "label": "Spanish",
            "value": "8000" 
          },
          { 
            "label": "Swedish",
            "value": "8500" 
          },
          { 
            "label": "Turkish",
            "value": "9000" 
          },          
          { 
            "label": "Ukranian",
            "value": "9500" 
          }
        ]
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];

