var win = Ti.UI.currentWindow;
var background = Ti.UI.createView();
var array=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'];
	
var view = Ti.UI.createView({
    backgroundColor:'black',
    
});
win.add(view);
   
   
var prevbtn = Ti.UI.createButton ({
	title: 'Prev',
	hight:"20%",
	width:"20%",
	bottom:0,
	left:0
})
prevbtn.addEventListener('click',function(e){
indexNumber -=1;
a2z.value = array[indexNumber];
});
view.add(prevbtn);

var nextbtn = Ti.UI.createButton ({
	title:'Next',
	hight:"20%",
	width:"20%",
	bottom:0,
	right:0
})
nextbtn.addEventListener('click',function(e){
indexNumber +=1;
a2z.value = array[indexNumber];
});
view.add(nextbtn);

var a2z = Ti.UI.createTextArea({
    value:array[0],
    font:{fontSize:50,fontFamily:'Ariel', fontWeight:'bold'},
    color:'red'
    
});



var  indexNumber = 0;
a2z.value = array[indexNumber];  

view.add(a2z);

