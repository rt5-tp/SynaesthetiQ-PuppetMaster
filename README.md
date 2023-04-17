# SynaesthetiQ-PuppetMaster
This is the top-level repository for the [SynaesthetiQ](https://github.com/rt5-tp) project. SynaesthetiQ is a music visualiser prototype using envelope following, frequency analysis and genre detection to create engaging visualisations of music in real-time. This project was started as part of a Real-time Embedded Programming undergraduate project at the University of Glasgow. 

![Render of the SynaesthetiQ](/assets/images/synaesthetiq-render.png)

## One Repo to Rule Them All

This repository is called the 'PuppetMaster' because it's main function is to link together other repositories, each containing different submodules of the full SynaesthetiQ processing flow. These submodules are:

* [**Audio Capture**](https://github.com/rt5-tp/SynaesthetiQ-audiocapture) - Handles data acquisition, frequency analysis and genre classification.
* [**Envelope Following**](https://github.com/rt5-tp/envelope_following) - A simple envelope follower to visualise  the pulse of music.
* [**LED Library**](https://github.com/rt5-tp/SynaesthetiQ-LED-LIbrary) - Renders visualisations to an array of LEDs.

The relationships between these submodules are handled by the [Visualiser](https://github.com/rt5-tp/SynaesthetiQ-PuppetMaster/tree/main/src/Visualiser), which is defined within this repository.

## Data Flow

The overall flow of data through these submodules can be representatd by this diagram:

<div align="center">
    <img width="100%" src="https://github.com/rt5-tp/.github/blob/main/Full%20Data%20Flow%20Diagram.svg">
</div>

More information on how these submodules work and interact with each other can be found in their respective repositories and the [documentation]() for this project.

## Installation

### Install dependencies for Raspberry Pi

Run the "setup.sh" script contained within the root of this repository
> sudo sh setup.sh

## Info

## Team

- [Ross McKeown](https://github.com/rossamck) 
- [Thomas Nordentoft](https://github.com/nordwestt) 
- [Thomas Burnip](https://github.com/TBurnip)
- [Neil Crawshaw](https://github.com/Neil-A-C)
