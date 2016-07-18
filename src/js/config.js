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
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];

