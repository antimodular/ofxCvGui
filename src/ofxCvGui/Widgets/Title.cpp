#include "Title.h"
#include "../Utils/Constants.h"
#include "ofxAssets.h"
namespace ofxCvGui {
	namespace Widgets {
		//----------
		Title::Title(string caption, Level level) :
		level(level) {
			this->setCaption(caption);
			this->setBounds(ofRectangle(5, 0, 100, 40));
			this->onDraw += [this] (ofxCvGui::DrawArguments & args) {
				int fontSize = 14;
				switch (this->level) {
				case H1:
					fontSize = 18;
					break;
				case H2:
					fontSize = 14;
					break;
				case H3:
					fontSize = 12;
					break;
				}
				auto font = ofxAssets::font(ofxCvGui::defaultTypeface, fontSize);

				font.drawString(this->caption, 0, this->getHeight() - 10);
			};
		}
	}
}