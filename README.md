# Faceless
A proof of concept OpenVR driver for using VR controllers without an HMD.

The goal is to guesstimate the position of the HMD based on the controller positions, so you can interact with the virtual world without needing to wear the HMD at all.

# How to use

Download either Linux or Windows builds from the release.

The output driver will go to `bin/drivers/faceless`. This driver (the whole faceless folder) can be put in `Steam/steamapps/common/SteamVR/drivers` for use.

To use this, you must open your `Steam/config/steamvr.vrsettings` file and set the `activateMultipleDrivers` key true under the `steamvr` sections:
```json
"steamvr" : {
      "activateMultipleDrivers" : true
}
```

This will allow the driver used for the VR controllers to work concurrently with this driver. Finally, enable the driver in the Startup section of SteamVR settings.

# Running Faceless

When Faceless is running, a H symbol will appear.

![image](https://user-images.githubusercontent.com/32321/147889239-12217cec-47c8-4b8d-b835-f15527eebc4d.png)

# Contributing to Faceless

```
git clone https://github.com/V-Sekai/V-Sekai-faceless.git
cd V-Sekai-faceless
mkdir -p build
cd build
cmake .. -G"Visual Studio 16 2019" # Can also use "Ninja"
cmake --build . --config RelWithDebInfo
# Copy faceless
```
