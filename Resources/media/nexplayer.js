var win = Ti.UI.currentWindow;

win.backgroundColor= 'white';

var nexplayer = require('org.appcelerator.nexplayer');
Ti.API.info("module is => " + nexplayer);


function getVideoPlayer()
{
	Ti.API.info('Java file should change');
  //nexplayer.launchnexplayer();
  
  Ti.Android.currentActivity.startActivity(Ti.Android.createIntent({
  		packageName:'com.nexplayer.nexplayersample',
        className: 'com.nexplayer.nexplayersample.NexPlayerSample'
    }));
}

setTimeout(getVideoPlayer,5000);
