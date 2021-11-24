# Reverse engineering the apple M1 frontend

## Problems with switching cores
I knew from Dougall Johnson's GitHub repository that I would need some kind of
MacOS kernel extension to access the system registers. I tried to write some
code that accessed `PMC0` anyway and got an error:
```
zsh: illegal hardware instruction
```
Each core has it's own set of performance counters and they have to be
configured before use. This is done by writing values into the `PMCR0` register.
The default value seems to be `0x7003403` (purely anecdotal conclusion), but
Dougall Johnson uses this value:
```
0x3003400FF4FF

Meaning of bits according to Asahi:
- [7:0]   Enable PMC0-7                           (0x0000000000FF)
- [10:8]  Set interrupt mode                      (0x000000000400)
- [11]    Clear PMI (interrupt) flag              (0x000000000000)
- [19:12] Enable PMI for PMC0-7                   (0x0000000FF000)
- [20]    Disable counting on a PMI               (0x000000000000)
- [22]    Block PMIs until after eret             (0x000000000000)
- [23]    Count global (not just core) L2C events (0x000000000000)
- [30]    User-mode access to registers enable    (0x000040000000)  !!!
- [33:32] Counter enable for PMC #9-8             (0x000300000000)
- [45:44] Enable PMI for PMC #9-8                 (0x300000000000)
```

Bit 30 is the most important for us, because if it is 0, it prevents us from
accessing any PMC related registers. Because it is 0 by default, we need to 
use a kernel module to set the bit.

## Installing PMCKext2
### Disable security
MacOS has some security features that do not allow you to load custom kernel 
extensions. We have to perform the same sort of security downgrade as when we 
were trying to install Asahi Linux.

First, boot into 1TR by holding the power button until you see an Apple logo on
the screen. Choose 'Options' under the gear icon and wait for 1TR to load. Once
you see a bar at the top of the screen, select 'Utilities > Terminal' to bring
up a console. Type in
```
# diskutil apfs listVolumeGroups | grep -B3 Linux | grep Group
```
and remember the ID that comes up. `This assumes you completed the partitioning
step for Asahi Linux`.

Downgrade security with
```
# bputil -nkcas
```
and run
```
# csrutil disable
```
If a 'Failed to create local policy' error pops up, run
```
# csrutil clear
```

### Download Xcode
I had to create an Apple account to access the App store and install Xcode. We 
need this to build the kernel extension that Dougall Johnson provides on his 
GitHub.

### Build and load the PMCKext2 extension
Good information on how to build and load kernel extensions in MacOS was 
difficult to find but I came across [this](Library/Developer/Xcode/DerivedData/PMCKext2-eglkrzbygupxewfbausorolyjblh/Build/Products/Debug/PMCKext2.kext) excelent guide which worked.
1. Create a new `Generic kernel extension` project in Xcode
    1. Name the project 'PMCKext2'
    2. Write in 'com.example' as the company name
    3. Select some location and finish

2. Set up kext project
    1. Copy dougallj's code into the c file that Xcode created
    2. Click 'Info' in the viewer on the left, then right click on the empty 
    space in the window and select `Raw Keys & Values`
    3. Right click the empty space again and select `Add Row`. In the name field
    type `CFBundleIdentifier`, which should autocomplete from a list
    4. Choose `com.example.PMCKext2` as the value

3. Build kext project
    1. Right click on the 'Info' file and select 'Open As > Source Code'
    2. Replace
    ```
        <key>OSBundleLibraries</key>
	    </dict>
    ```
    with
    ```
    <key>OSBundleLibraries</key>
	<dict>
		<key>com.apple.kpi.bsd</key>
		<string>21.1</string>
		<key>com.apple.kpi.libkern</key>
		<string>21.1</string>
	</dict>
    ```
    3. Click on the 'Play' icon in the upper left to build the kext. The kext
    bundle that we need will be located in
    ```
    ~/Library/Developer/Xcode/DerivedData/PMCKext2-.../Build/Products/Debug/PMCKext2.kext
    ```
    by default.

4. Load the kext
    1. To load ta kext, it needs to belong to sudo. Run
    ```
    sudo cp -R /path-to/PMCKext2.kext /tmp
    sudo kextutil /tmp/PMCKext2.kext
    ```
    2. If you are loadin this kext for the first time, a popup will ask you to
    authorize it in the settings.
    3. After authorization, reboot the mac and repeat the above two commands.

