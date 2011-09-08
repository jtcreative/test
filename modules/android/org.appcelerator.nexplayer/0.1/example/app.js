// This is a test harness for your module
// You should do something interesting in this harness 
// to test out the module and to provide instructions 
// to users on how to use it by example.


// open a single window
var window = Ti.UI.createWindow({
	backgroundColor:'white'
});
var label = Ti.UI.createLabel();
window.add(label);
window.open();

// TODO: write your module tests here
var nexplayer = require('org.appcelerator.nexplayer');
Ti.API.info("module is => " + nexplayer);

label.text = nexplayer.example();

Ti.API.info("module exampleProp is => " + nexplayer.exampleProp);
nexplayer.exampleProp = "This is a test value";

if (Ti.Platform.name == "android") {
	var proxy = nexplayer.createExample({message: "Creating an example Proxy"});
	proxy.printMessage("Hello world!");
}

function getVideoPlayer()
{
  nexplayer.launchnexplayer();
    //Ti.Android.currentActivity.startActivity(Ti.Android.createIntent({
  		//packageName:'org.appcelerator.nexplayer.nexplayersample',
      //  className: 'org.appcelerator.nexplayer.nexplayersample.NexPlayerSample'
    //}));
}

setTimeout(getVideoPlayer,5000);