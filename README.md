# OscLiveLink

Using the Hallway Tile standard, this plugin receives OSC messages and applies them as a [LiveLink](https://docs.unrealengine.com/5.0/en-US/live-link-in-unreal-engine/) source to animate [MetaHumans](https://www.unrealengine.com/en-US/metahuman) in Unreal.

<img align="left" width="100%" src="https://user-images.githubusercontent.com/85010195/191375085-08470afc-e564-4d1c-bb59-14270e71dc95.gif"><br/><br/><br/><br/>
_Example using web platform and local Bridge application on Windows_
 
# Installation

- To install the plugin, download the latest release from the [Releases page](https://github.com/Launchvox/OscLiveLink/releases) on Github.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191378043-4eaf5732-f50f-4941-b5f4-8aff4091a107.png">

- Create a new folder named "Plugins" in the root of your Unreal Engine Project.

<img width="75%" src="https://user-images.githubusercontent.com/85010195/191376290-61fcc5c7-4841-4ea4-a4e2-1c8ec410483f.png">

- Unzip the contents of the latest release ZIP file downloaded from Github into the Plugins folder.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191377169-5b1106a0-8137-4976-97dd-e83a2e6d2f73.png">

__Note:__ If you're working with a different version of Unreal (such as on Mac instead of PC), you'll need your project to be a [C++ project](https://docs.unrealengine.com/5.0/en-US/unreal-engine-cpp-quick-start/) in order to compile the binaries for your platform. Adding a new C++ class before installing the plugin will convert a Blueprint-only project to a C++ project.

<img width="75%" src="https://user-images.githubusercontent.com/85010195/191379192-36afba1f-0a66-495b-8601-869ecc55cfd6.png">

- Open your Unreal project and navigate to `Edit>Plugins>Project`. 
If installed properly, the plugin will appear and be enabled by default.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191379985-27405a81-8a7a-4c48-a93e-03dddec7475b.png">

Usage
---
__Note:__ Be sure to read through both this section and __AR Kit Bugfix__ if your are using Unreal `5.0.1`.

- To use the plugin, start the [Hallway Tile application](https://joinhallway.com/download/) and enable OSC Streaming. 
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191381594-d3daad60-59d3-4b6f-a1b6-2a653ba70de5.png">

- This plugin acts as a _host_ for the Hallway application _client_. This means you will need to configure the correct _IP address_ and _port_ in both the Hallway application and Unreal Engine to share information between the two. You can configure the plugin's settings by navigating to `Edit > Project Settings > Plugins > LiveLink OSC`.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191384938-d7a64ca4-8c6d-4b9b-9d40-4745d94961a9.png">

- If both applications are on the same computer, then you can use the localhost address: `127.0.0.1` for Hallway Tile's _Host_, and `0.0.0.0` for Unreal's _Listening Address_. (`0.0.0.0` represents any possible address.)

- If Hallway and Unreal are running on different computers in the same network, use `0.0.0.0` **or** the IP address for the Hallway computer as Unreal's _Listening Address; and use the Unreal computer's IP address as Hallway's _Host_. 

__Note:__ Look up how to find [your local IP address](https://www.google.com/search?q=how+to+lookup+ip+address&rlz=1C1ONGR_enUS999US999&oq=how+to+lookup+local+IP+address) online if you're not familiar.

- The default port is `9001`. This can remain unchanged unless another application requires that port.

- To use the OSC source, once per project you will need to navigate to `Window > Virtual Production > LiveLink`
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191373457-189224df-7795-49b2-83f5-1ec321e1ccc5.png">

- Add the source by pressing `+ Source > Message Bus Source > Hallway Tile OSC`
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191387220-e403e9ff-a3c1-435e-97b8-4a5abc7a51c3.png">

- It will appear in the bottom left list of LiveLink sources. If the dot is green, everything is working properly. If the dot is yellow, there may be a address/port misconfiguration or a firewall blocking the OSC stream.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191387821-76b578a3-0ef5-4187-ac83-e4abea4a768a.png">

- `Halway Tile OSC` will now appear as an option in the list of possible options when you view your Metahuman's Detail's panel in `Outliner > Default > LLink Face Subj`.
<img width="75%" src="https://user-images.githubusercontent.com/85010195/191388464-23dce082-3e70-4a19-8c2a-60b3405fcf13.png">

- Use the `Simulate` play mode to control your camera while projecting face shapes on your Metahuman Character. You're all set!
<img align="left" width="100%" src="https://user-images.githubusercontent.com/85010195/191389437-f60fddf2-3eee-43b1-a0ca-990ab9f95764.gif">
_Subject selected, running in simulate mode with AR Kit Bugfix_

AR Kit Bugfix
---
There is a bug in Unreal's `5.0.1` version of Metahumans that has a missing blenshape name. This offsets most blendshapes to the wrong order, completely breaking the effect of face capture. This is not unique to Hallway Tile, it applies to AR Kit LiveLink as well. The solution to this bug involves copying the pose library from Unreal `4.27` to Unreal `5.0.1` and is described [here](https://forums.unrealengine.com/t/live-link-facial-animation-deformed-after-new-metahuman-release/578853/27?page=2).

Web Demo
---
If you're running the [web-version](https://joinhallway.com/demo/) of Hallway, you'll need to contact [Hallway](https://joinhallway.com) and request the _Bridge_ application to convert from the web's _TCP_ to the _UDP_ protocol. (If you're using the Mac version of HallwayTile, it will broadcast _UDP_ directly.)

<img width="75%" src="https://user-images.githubusercontent.com/85010195/191382689-29dc60e1-0274-4f3f-8771-9bf9bdd76a73.png">

The web demo will broadcast TCP to port `9000`, and the bridge application will convert it from TCP to UDP and forward to port `9001` by default.
