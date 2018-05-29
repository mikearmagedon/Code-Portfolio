var ws = new WebSocket(url("/ws"));
function url(s) {
	var l = window.location;return ((l.protocol === "https:") ? "wss://" : "ws://") + l.hostname + s;
}
window.onunload = loggingOut;
$(document).on('click', '#ui-id-1', function(){
	$("#ui-id-1").css('background', '#ffffff');
});
$(document).on('click', '#ui-id-2', function(){
	$("#ui-id-2").css('background', '#ffffff');
});
$(document).on('click', '.person', function(){
	var auxNick = $(this).text();
	auxNick = auxNick.substr(1);
	auxNick = auxNick.split('\n', 1);
	var auxMsg = $('#TxTBox').val();
	if ($('#TxTBox').val() == "") {
		$('#TxTBox').val("\\pvt" + auxNick + ":");}
	else{
		var i = auxMsg.indexOf(':');
		var aux2 = auxMsg.substr(i);
		auxMsg = auxMsg.split(':', 1);
		auxMsg += ';' + auxNick + aux2;
		$('#TxTBox').val(auxMsg);
	}
});
function playSound() {
    mySong = document.getElementById("myAudio");
    mySong.autoplay = true;
    mySong.load();
}
ws.onopen = function() {
	ws.send("\\usr" + nick + "\0");
}
ws.onmessage = function(serverData) {
	var msg = serverData.data;
	var typeMsg = msg.substr(0, 4);
	if (typeMsg === "\\onU") {
		printOnlineBox(msg.substr(4, ((msg.length)-1)));
	}
	else if (typeMsg === "\\msg") {
		printChatWindow(msg.substr(4));
	}
	else if (typeMsg === "\\pvt")
		printPrivateChatWindow(msg.substr(4));
}
function printOnlineBox(uNick) {
	var now = 0;
	var before;
	var subNick;
	while (now < uNick.length-1)
	{
	before = now;
	now = uNick.indexOf(';', before+1);
	if (before === 0) {
		subNick = uNick.substr(before, now);
		document.getElementById("OnlineBox").innerHTML = "";
	}
	else subNick = uNick.substr(before+1, (now-before)-1);
		if (subNick !== nick)$("#OnlineBox").append("<div class= 'person'> <font  color='green'>" + subNick + "\n" + "<b></b></font></div>");
	}
	var objDiv = document.getElementById("OnlineBox");
	objDiv.scrollTop = objDiv.scrollHeight;
}
function printChatWindow(itext) {
	var i = 0;
	for (; itext[i] != ':'; i++);
		var mNick = itext.substr(0, i);
	itext = itext.substr(i+1);
	var objDiv = document.getElementById("chatWindow");
	var allowScroll = 0;
	if (objDiv.scrollTop === objDiv.scrollHeight - 425)
		allowScroll = 1;
	if (mNick !== nick)$("#chatWindow").append("<div class='msg'><b>" + mNick + "</b>: " + itext + "</div>");
	else $("#chatWindow").append("<div class='own'><b>me</b>: " + itext + "</div>");
	if ((document.getElementById("chatWindow")).getAttribute("aria-hidden")=="true" && mNick != nick){
		playSound();
		$("#ui-id-1").css('background', 'linear-gradient(#f5934e, white)');
	}
	if (allowScroll)
		objDiv.scrollTop = objDiv.scrollHeight;
}
function printPrivateChatWindow(itext) {
	var i = 0;
	for (; itext[i] != ':'; i++);
		var mNick = itext.substr(0, i);
	itext = itext.substr(i+1);
	var objDiv = document.getElementById("privateChatWindow");
	var allowScroll = 0;
	if (objDiv.scrollTop === objDiv.scrollHeight - 425)
		allowScroll = 1;
	if (mNick !== nick)$("#privateChatWindow").append("<div class='pmsg'><b>" + mNick + "</b>: " + itext + "</div>");
	else $("#privateChatWindow").append("<div class='own'><b>me</b>: " + itext + "</div>");
	if ((document.getElementById("privateChatWindow")).getAttribute("aria-hidden")=="true" && mNick != nick){
		playSound();
		$("#ui-id-2").css('background', 'linear-gradient(#f5934e, white)');
	}
	(allowScroll)
	objDiv.scrollTop = objDiv.scrollHeight;
}
function sendMsgToServer() {
	var content =  $("#TxTBox").val();
	var i = 0;
	while((content[i] == " ") || (content[i] == "\n")){
		i++;
	}
	content = content.substr(i);
	var contentType = content.substr(0, 4);
	if (contentType == "\\pvt"){
		i = content.indexOf(':');contentType = content.split(':', 1);
		var contentMsg = content.substr(i+1);
		i=0;
		while((contentMsg[i] == " ") || (contentMsg[i] == "\n")){
			i++;
		}
		contentMsg = contentMsg.substr(i);
		if((contentMsg != '') && (contentMsg != " ") && (contentMsg != "\n") && (contentMsg != "\0")){
			ws.send(contentType + "_" + nick + ":" + contentMsg  + "\0");
		}
	}else if((content != '') && (content != " ") && (content != "\n") && (content != "\0")){
		ws.send("\\msg" + nick + ":" + content + "\0");
	}
	document.getElementById("TxTBox").value = "";
}
function loggingOut() {
	ws.send("\\off" + nick + "\0");
}
$(function() {
	$("#tabs").tabs();
});