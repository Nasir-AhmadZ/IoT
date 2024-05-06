// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePart1 = F(R"=====(
  <!DOCTYPE html>
<html lang="en">
<head>
    <title>Video</title>
    <link rel="stylesheet"href="style.css"><!--External CSS-->
    <!--Internal CSS-->
    <style>
        
    </style>
</head>
    
<body>
    <div id="VideoMain"> 
        <iframe width="560" height ="315" src="http://192.168.0.67:81/stream" frameborder="0" alt="camera video stream"allowfullscreen></iframe>
    </div>
    <div class="buttons">
        <h2>Water gun</h2>
        <button class="myButton" onclick="fetch('/sentKeyPressToWebServer?button=ON')">on</button>
        <button class="myButton" onclick="fetch('/sentKeyPressToWebServer?button=OFF')">off</button>
    </div>
    

</body>
</html>
)=====");

