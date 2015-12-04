# RandomInputPrank
Reads messages from a server and simulates keystrokes to type them, or types random messages at random times

This project is for Visual Studio 2015 and should compile/run for any Windows version (I haven't actually tested that, though)

___

To install this on a machine, compile rip.exe from the Visual Studio Project and paste it into the **install** folder in the root of the project. Then run install.bat on your victim's computer.

The install folder contains a defaultsettings.ini file which defines the default settings enabled for your install. When install.bat is run, defaultsettings.ini overwrites the current settings.ini for this program on the victim's computer.

___

This program has 3 parts:

1. the program itself that runs on the victim's computer
2. the API
3. the message poster page

By default, the program uses my hosted API that is already set up on a server. However, if you want to host the API yourself to store just *your* messages, you can copy the files in the **api_src** folder to a php web server and change the api_endpoint setting in your settings.ini to the url of the folder containing your hosted API. You will also have to change the api_endpoint variable in **msg_poster_src/prank_form.js** if you want the message poster to post messages to your API.

The message poster page is inside the **msg_poster_src** folder of the project. You can open index.html in a browser to post messages for the program to recieve.

___

settings.ini properties
* random
    - must be true or false
    - default is true
    - defines whether the program should type a random character/phrase on each interval
* random_chance
    - must be a real number between 0 and 1
    - default is 0.3
    - defines the likelihood of a random character/phrase being typed on each interval
* retrieve
    - must be true or false
    - default is true
    - defines whether the program should check the api endpoint for messages on each interval
* api_endpoint
    - must be a url to a server that is hosting the api (available in the api_src folder in the root of the project)
    - default is [http://fojam.users.sourceforge.net/api/inputprank](http://fojam.users.sourceforge.net/api/inputprank)
    - defines the api endpoint for retrieving messages. An advantage to hosting it yourself is that it will only host your posted messages, rather than anyone's posted messages
* phrases
    - must be a json encoded array of strings (remember to escape special ini characters)
    - default is []
    - defines a list of phrases to output instead of just random characters. If phrases is not empty, setting random to true will use a random phrase from the list instead of a random character
* refresh_interval
    - must be an integer
    - default is 300000
    - defines the amount of time in milliseconds to wait each time before checking the server or randomly deciding whether to input a random phrase/character
