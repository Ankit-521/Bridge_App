

const configuration = {
	iceServers:[{urls:'stun:stun2.1.google.com:19302'}]
}

let localPeerConnection;
let localStream
let mySide = -1; // store user side


// === set player side on the table
const setSide=(side)=>{
	console.log("setting side "+side)
mySide=side;
}

//=====Expose roomManager object to js==========================
// This function is called from QML
const connect=()=>{
new QWebChannel(qt.webChannelTransport,(channel)=>{
	var jsObj=channel.objects.roomManager;
	window.roomManager=jsObj;
	jsObj.jsInvokeRequest.connect(invokeJsFunction)
	console.log("done")	
})	
}

//========================================================================
// Set up Camera and audio resources
// This function is called from QML
const useCamera = async ()=>{

	try{
		localStream = await navigator.mediaDevices.getUserMedia({
			video:true,
			audio:true
		});

		document.querySelector('#videoStream').srcObject = localStream;
	      createConnection();
	}
	catch(e){
	errorOccured(e)	
	}
	
}


//=======================================================
// Create a Peer connection object
const createConnection = ()=> {

localPeerConnection = new RTCPeerConnection(configuration);

localPeerConnection.onTrack = (event)=>{
	console.log("a new track is recieved ")
	console.log("a track is recieveds")
}

localPeerConnection.onicecandidate = (event)=>{
	console.log("an ice candidate is recieved \n  "+JSON.stringify(event.candidate))
	
}

for (const track of localStream.getTracks()) {
	console.log("adding track ")
	localPeerConnection.addTrack(track)
}

console.log("wfwefwe we we  "+roomManager)
roomManager.peerConnectionReady(mySide);

}


// call js function from c++
const invokeJsFunction=(type,args)=>{
console.log("callinf "+type+"  "+JSON.stringify(args))
switch(type){
case 0:
handleOffer(args)
break;

}
}


const handleOffer = (offerData) =>{
	
localPeerConnection.setRemoteDescription(new RTCSessionDescription(offerData.offer), ()=>{

	if(localPeerConnection.remoteDescription.type === 'offer'){

		console.log("handling an offer and creating an answer")

		localPeerConnection.createAnswer( (answer) => {

	console.log("creating an answer")

	localPeerConnection.setLocalDescription( answer, () => {

		console.log("sending answer")

		 // send answer to the requesting player
		roomManager.handleAnswer(answer, offerData.table, offerData.player);
	
	},errorOccured);

       },errorOccured)
	}
})
console.log("handleing offer "+JSON.stringify(offerData))

} 

const errorOccured = (error) => {
console.log("an error has occured")
}

