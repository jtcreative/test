/*var win = Ti.UI.currentWindow;
var bar = Titanium.UI.createButtonBar({
	labels:['Episodes', 'Most Viewed', 'Top Rated', 'Related'],
    backgroundColor:'#336699',
    bottom:0,
    height:50,
});
win.add(bar);
var button = Ti.UI.createButton({
	height: 70,
	top: 50,
	title:'Original'
});
button.media = 'http://adamelife.sandbox.phxx.com/statics/OttWeb/james/h264.mp4';
win.add(button);

var button1 = Ti.UI.createButton({
	height:70,
	top:200,
	title:'Flash'
});
button1.media = 'http://adamelife.sandbox.phxx.com/statics/OttWeb/james/flash.flv';
win.add(button1);

var button2 = Ti.UI.createButton({
	height:70,
	bottom:100,
	title:'New MP4'
});
button2.media = 'http://adamelife.sandbox.phxx.com/statics/OttWeb/james/mp4v.mp4';
win.add(button2);

var playMedia = function(e){
	
	var media = Ti.Media.createVideoPlayer({
		url:e.source.media
	});
	media.play();
}

button.addEventListener('click',playMedia);
button1.addEventListener('click',playMedia);
button2.addEventListener('click',playMedia);*/
