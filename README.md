# Multi thread cameras in OpenCV (with neglectable latency)
![output image]( https://qengineering.eu/github/MultiCamCodeBlocks.webp )<br>
### RTSP - UDP - TCP streams 
It is a known issue with RTSP streams and time-consuming algorithms such as deep learning frameworks. You're getting out of sync if individual frames take longer than your stream's frame rate to process. There is an increasing delay between reality and the captured images.<br>
There are two possible solutions to this problem.<br>
You could determine the missed images and skip them before grabbing a new frame. This solution is [implemented here](https://github.com/Qengineering/RTSP-with-OpenCV).<br>
Or you can continuously grabbing images in a seperated thread. This way, it is possible to connect more than one camera to your application.<br><br>
The code speaks for itself. You can either use GStreamer of FFmpeg to open the stream.<br>
The number of cameras connected is not limited. It could be even just one camera.<br>
However, the more cameras, the more CPU time is needed to visit all seperate threads.<br>

------------

## Dependencies.
To run the application, you have to:
- OpenCV installed [32-bit](https://qengineering.eu/install-opencv-4.5-on-raspberry-pi-4.html) or [64-bit](https://qengineering.eu/install-opencv-4.5-on-raspberry-64-os.html) OS<br/>
- Code::Blocks installed. (```$ sudo apt-get install codeblocks```)

------------

## Tips.
Use only a ***wired*** Ethernet connection for your RTSP stream. Wi-Fi can be unstable.<br>
Because the RTSP protocol is sensitive to even just one missing frame, the stream can easily crash.<br><br>
If you are using the stream in a deep learning app, adjust your resolution and frame rate to the requirements of the deep learning model.<br>
It is not a good idea to send a 1280x960 stream at 30 FPS if your model has a 416x416 input and takes 200 mSec to process a single frame.<br>
It only costs extra memory and processing power.<br><br>
Make sure all cameras have the same resolution. In the example, the OpenCV concatenate algorithm is sensitive to different resolutions. It throws an exception when it faces different widths or heights.<br><br>
Please note that the cameras are not synchronized with each other by the threads. They may have slightly different lags.<br><br>
If you want to stream UDP or TCP, are sure the streams work with the command line prompt beforehand. If not, they certainly won't work in OpenCV. Often errors are caused by the coding in the pipeline, the addresses or missing modules. If you need to install additional GStreamer modules, you'll need to rebuild your OpenCV also! For more information, see our [website](https://qengineering.eu/install-gstreamer-1.18-on-raspberry-pi-4.html).

------------

## Running the app.
To run the application load the project file MultiThread.cbp in Code::Blocks.<br><br> 
If you are using a **Jetson Nano**, you have to change the location where OpenCV has stored its header files to `/usr/include/opencv4`<br><br>
At line 20 in main.cpp the stream is defined.
#### RTSP
```
Grb1->Init("rtsp://192.168.178.129:8554/test/");
```
#### UDP
```
Grb1->Init("udpsrc port=5200 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink", cv::CAP_GSTREAMER);
```
Sender: RaspiCam with Raspberry Pi **Buster** OS<br>
`gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=-1 ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=192.168.178.84 port=5200`<br><br>
Sender: RaspiCam with Raspberry Pi **Bullseye** OS<br>
`gst-launch-1.0 -v libcamerasrc ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=192.168.178.84 port=5200`<br><br>
Note, `host=192.168.178.84` being the IP address of the receiver.

#### TCP
```
Grb1->Init("tcpclientsrc host=192.168.178.129 port=5000 ! jpegdec ! videoconvert ! appsink", cv::CAP_GSTREAMER);
```
Sender: RaspiCam with Raspberry Pi **Buster** OS<br>
`gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=-1 ! video/x-raw,width=640,height=480, framerate=30/1 ! videoconvert ! jpegenc ! tcpserversink  host=192.168.178.32 port=5000`<br><br>
Sender: RaspiCam with Raspberry Pi **Bullseye** OS<br>
`gst-launch-1.0 -v libcamerasrc ! video/x-raw,width=640,height=480, framerate=30/1 ! videoconvert ! jpegenc ! tcpserversink  host=192.168.178.32 port=5000`<br><br>
Note, `host=192.168.178.32` being the IP address of the sender.

#### RaspiCam (Bullseye)
```
Grb1->Init("libcamerasrc ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! videoscale ! video/x-raw, width=640, height=480 ! appsink", cv::CAP_GSTREAMER);
```
#### RaspiCam (Buster)
```
Grb1->Init("v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! videoscale ! video/x-raw, width=640, height=480 ! appsink", cv::CAP_GSTREAMER);
```
#### RaspiCam (only Buster or Bullseye legacy stack)
```
Grb1->Init(0);
```
#### Webcam (only Buster OS or Bullseye legacy stack)
```
Grb1->Init(0); //if RaspiCam is not connected
Grb2->Init(1); //if RaspiCam is connected
```
------------
[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 
