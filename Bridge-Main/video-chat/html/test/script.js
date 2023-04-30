// Generate random room name if needed
if (!location.hash) {
  var f = Math.floor(Math.random() * 0xFFFFFF).toString(16);
}
console.log("my rmmow wd " +f)
const roomHash = f//glocation.hash.substring(1);

console.log("my room has is s"+roomHash)
// TODO: Replace with your own channel ID
const drone = new ScaleDrone('2xmbUiTsqTzukyf7');

console.log("m,y od d "+drone)
// Room name needs to be prefixed with 'observable-'
const roomName = 'observable-' + roomHash;
const configuration = {
  iceServers: [{
    urls: 'stun:stun.l.google.com:19302'
  }]
};
let room;
let pc;


function onSuccess() {};
function onError(error) {
  console.log("my error is  "+error)
  console.error(error);
};

drone.on('open', error => {
  if (error) {
    console.log("is error")
    return console.error(error);
  }
  else{
    console.log("no error")
  }
  room = drone.subscribe(roomName);
  room.on('open', error => {
    if (error) {
      onError(error);
    }
    else{
console.log("is opene")

    }
  });
  // We're connected to the room and received an array of 'members'
  // connected to the room (including us). Signaling server is ready.
  room.on('members', members => {
    console.log('MEMBERS', members);
    // If we are the second user to connect to the room we will be creating the offer
    const isOfferer = members.length === 2;
    startWebRTC(isOfferer);
  });
});

// Send signaling data via Scaledrone
function sendMessage(message) {
  drone.publish({
    room: roomName,
    message
  });
}

const  startWebRTC=async (isOfferer)=> {
  console.log("stating webrct")
  pc = new RTCPeerConnection(configuration);

  // 'onicecandidate' notifies us whenever an ICE agent needs to deliver a
  // message to the other peer through the signaling server
  pc.onicecandidate = event => {
     console.log("is iced")
 
    if (event.candidate) {

      sendMessage({'candidate': event.candidate});
    }
  };

  // If user is offerer let the 'negotiationneeded' event create the offer
  if (isOfferer) {
       console.log("is offered")
    pc.onnegotiationneeded = () => {
         console.log("iosos")
      pc.createOffer().then(localDescCreated).catch(onError);
    }
  }

  // When a remote stream arrives display it in the #remoteVideo element
  pc.onaddstream = event => {
       console.log("new sreamas")
    remoteVideo.srcObject = event.stream;
  };

let stream;
try{
 stream= await navigator.mediaDevices.getUserMedia({
    audio: true,
    video: true,
  })
  localVideo.srcObject = stream;
    // Add your stream to be sent to the conneting peer
    pc.addStream(stream);
}catch(e){
onError(e)
}
  

  // Listen to signaling data from Scaledrone
  room.on('data', (message, client) => {
    // Message was sent by us
    console.log("my data us  ")
    if (client.id === drone.clientId) {
      return;
    }

    if (message.sdp) {
      // This is called after receiving an offer or answer from another peer
      pc.setRemoteDescription(new RTCSessionDescription(message.sdp), () => {
        // When receiving an offer lets answer it
        if (pc.remoteDescription.type === 'offer') {
          pc.createAnswer().then(localDescCreated).catch(onError);
        }
      }, onError);
    } else if (message.candidate) {
      // Add the new ICE candidate to our connections remote description
      pc.addIceCandidate(
        new RTCIceCandidate(message.candidate), onSuccess, onError
      );
    }
  });
}

function localDescCreated(desc) {
  pc.setLocalDescription(
    desc,
    () => sendMessage({'sdp': pc.localDescription}),
    onError
  );
}
