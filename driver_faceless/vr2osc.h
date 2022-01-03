// MIT License

// Copyright (c) 2022 K. S. Ernest (iFire) Lee & V-Sekai
// Copyright (c) 2020 Kota Iguchi

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unordered_map>

struct vr2osc_options
{
	std::string rootbone;
	bool motion_in_place = false;
	bool record_facial_expression = false;
	std::chrono::milliseconds interval;
	std::uint8_t fps = 0;
};

class Vr2OscPacketListener : public osc::OscPacketListener {
private:
	std::chrono::steady_clock::time_point lasttime_checked;
	std::uint32_t frame_count = 0;
	vr2osc_options options;
    
    struct vr2osc_traverse_state
    {
        bool loaded;
        bool received;
        std::uint32_t channels_count;
        std::uint8_t  indent;
        float* translation;
        float* rotation;
    } ;
	vr2osc_traverse_state state;
public:
	Vr2OscPacketListener(const vr2osc_options options) : osc::OscPacketListener()
		, options(options)
		, state{ false, false, 0, 0, nullptr, nullptr }
		, lasttime_checked()
		, frame_count(0)
	{
	}

	virtual ~Vr2OscPacketListener()
	{
	}

	virtual void ProcessMessage(const osc::ReceivedMessage& m,
		const IpEndpointName& remoteEndpoint)
	{
		(void)remoteEndpoint; // UNUSED

		auto arg = m.ArgumentsBegin();
		const auto address = m.AddressPattern();
		if (!state.loaded && std::strcmp(address, "/VMC/Ext/OK") == 0 && arg->IsInt32()) {
			const auto loaded = (arg++)->AsInt32Unchecked();
			const auto calibrated = (arg++)->AsInt32Unchecked();
			if (loaded == 1 && calibrated == 3) {
				state.loaded = true;
			}
		}
		else if (!state.received && state.loaded && std::strcmp(address, "/VMC/Ext/VRM") == 0) {
			if (arg->IsString()) {
				const auto value = (arg++)->AsStringUnchecked();
				if (strlen(value) > 0) {
					std::cout << "[INFO] Start recording..." << std::endl;
					state.translation; // TODO Add a translation
					state.rotation; // TODO Add a rotation

					frame_count++;

					state.received = true;					
				}
			}
		}

		if (state.received) {
			const auto time = std::chrono::steady_clock::now();
			const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(time - lasttime_checked);
			if (delta > options.interval) {
				lasttime_checked = time;
				frame_count++;
			}
		}
	}
};