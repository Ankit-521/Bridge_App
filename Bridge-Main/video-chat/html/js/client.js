
// Change this stun server during productio
const configuration = {
	iceServers:[{urls:'stun:stun2.1.google.com:19302'}]
}

let localPeerConnection;
let localStream
let mySide = -1; // store user side
// RTC peer connection
localPeerConnection = new RTCPeerConnection(configuration)


localPeerConnection.onTrack = (event)=>{
	console.log("a new track is recieved ")
	console.log("a track is recieveds")
}

localPeerConnection.onicecandidate = (event) => {
	console.log(" an ice candidate is recieved \n  "+JSON.stringify(event.candidate))
	
}

// expose mainWindow object to js
const connect=()=>{
new QWebChannel(qt.webChannelTransport,(channel)=>{
	var jsObj=channel.objects.mainWindow;
	window.mainWindow=jsObj;

	jsObj.jsInvokeRequest.connect(invokeJsFunction)
	console.log("done")
	
})	
}

// get video streams
const getCamera = async ()=>{

	try{
		localStream = await navigator.mediaDevices.getUserMedia({
			video:true,
			audio:true
		});
			document.querySelector('#videoStream').srcObject = localStream;
console.log("calling callbalc")
       startWebrtc()

		}
	catch(e){
		console.log('my error is --------------  '+e);
		errorOccured(e)
	}
}

const startWebrtc = ()=>{

console.log("i call back")



for (const track of localStream.getTracks()) {
	console.log("adding track ")
	localPeerConnection.addTrack(track)
}

console.log("done setting up webrtc  "+roomManager)
//mainWindow.peerConnectionReady();





}


// invite player to join multiplayer room
const invitePlayer = (table,player) => {

 localPeerConnection.createOffer( (offer) => { 

//console.log("done settin offers "+JSON.stringify(offer));
		 localPeerConnection.setLocalDescription(offer)
		console.log("done settin offers "+JSON.stringify(offer));

		mainWindow.invitePlayer(player,table,offer);


	},errorOccured)

}


const invokeJsFunction = (peerType, data, player,table)=>{

	console.log("my dta a si "+peerType+"  "+data+" "+player+" "+table)
	switch(peerType){
		case 0:
		break;

		case 1:
handleAnswer(data,player,table)
		break;

	}
}

const handleAnswer = (answer,player,table)=>{
	console.log("handleing anser")
localPeerConnection.setRemoteDescription(new RTCSessionDescription(answer), (s)=>{
	console.log("is succucess")
},errorOccured);

}

const errorOccured = (error)=>{
	console.log("an error has occrues "+error)
}

