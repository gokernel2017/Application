//-------------------------------------------------------------------
//
// Application API(0.1.0): WEB MODE
//
// START DATE: 26/05/2018 - 10:40
//
//-------------------------------------------------------------------
//
// not used in WEB MODE:
var argc = 0;
var argv = 0;
//
var zIndex = 1;
var isTouch = false;

function app_init (ac, av) {
  isTouch = (('ontouchstart' in window) || (navigator.MaxTouchPoints > 0) || (navigator.msMaxTouchPoints > 0));
  if (document.getElementsByTagName("body").length==0 || document.getElementsByTagName("link").length==0) {
    alert ("INIT ERRO: tag: <body> or tag: <link rel='stylesheet' href='garden.css' /> not found");
    return 0;
  }
  return 1;
}

function app_new_window (x,y,w,h) {
  var o=document.createElement("widget");
//  var b=document.createElement("titlebar");

//  o.setAttribute("id", "main_body");

  o.style.left = x+'px';
  o.style.top = y+'px';
  o.style.width = w+'px';
  o.style.height = h+'px';

//  o.appendChild(b); // add title
  document.body.appendChild(o); // add here

  return o;
}
