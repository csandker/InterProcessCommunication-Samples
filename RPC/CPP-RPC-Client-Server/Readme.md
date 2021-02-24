## Sample VC++ RPC Client & Server

This sample implementation is based on my RPC post here:<br>
[Offensive Windows IPC Internals 2: RPC](https://csandker.io/2021/02/21/Offensive-Windows-IPC-2-RPC.html)

A few things to note here:<br>
It makes sense to have the Interface Definition in a seperate “Projekt” cause the MIDL generates headers for both client and server

You can avoid adding an extra ACF file if its just the Binding Handle Type you want to set, if you want some more features, e.g. [asynchronous communication](https://docs.microsoft.com/en-us/windows/win32/rpc/asynchronous-rpc) you will need an ACF file.<br>
I've the set binding handle type within the IDL file, which requires the following setting switch:
**You need to specify the “/app_config” MIDL flag under "Configuration Properties" -> MIDL -> General -> Set "Application Configuration Mode" to “Yes (/app_config)”**

As your RPC Client/Server will call some RPC specific API function you need to let your Linker know where to find these function. If you don't include rpctXX you will get errors LNK2019 Linker errors like “error LNK2019: unresolved external symbol __imp_RpcBindingFree ”. To add the RPC library go to:<br>
**“Configuration Properties” -> “Linker” -> “Input” -> Edit the “Additional Dependencies” menu item and add “rpcrt4.lib”**

When you write your RPC Client or Server in VC++ you proberly need to adjust the “C Compiler Char Type” of your IDL project. The MIDL.exe will output C Source and Header files and your VC++ Server will call include these. If you get a Linker LNK2001 error saying it can't resolve an external symbol for an Interface function it's probably mixed character type issue. To do that go to:<br>
**"Configuration Properties" -> MIDL -> General -> Set “C Compiler Char Type” to “Ascii (/char ascii 7)"**

You might need to configure the Character set of your client and server code to use Unicode in order to allow for correct type casting.
To do that go to:<br>
**“Configuration Properties” -> General -> Set “Character Set” to “Use Unicode Character Set”**