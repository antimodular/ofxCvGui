#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseImage : public Base {
		protected:
			BaseImage();
			void drawPanel(const DrawArguments& arguments);
			virtual void drawImage(const DrawArguments& arguments) = 0;
			bool refreshPerFrame; ///<needs to become a button
		};
	}
}