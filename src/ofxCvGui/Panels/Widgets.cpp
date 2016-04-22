#include "Widgets.h"


using namespace ofxCvGui::Widgets;

namespace ofxCvGui {
	namespace Panels {
		//----------
		shared_ptr<ofxCvGui::Widgets::LiveValueHistory> Widgets::addFps() {
			auto widget = ofxCvGui::Widgets::makeFps();
			this->add(widget);
			return widget;
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::LiveValueHistory> Widgets::addMemoryUsage() {
			auto widget = ofxCvGui::Widgets::makeMemoryUsage();
			this->add(widget);
			return widget;
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Button> Widgets::addButton(const string & caption, const function<void()> & action, char hotKey) {
			return this->add(new ofxCvGui::Widgets::Button(caption, action, hotKey));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Indicator> Widgets::addIndicatorBool(const string & caption, const function<bool()> & get) {
			return this->add(new ofxCvGui::Widgets::Indicator(caption, [get]() {
				if (get()) {
					return ofxCvGui::Widgets::Indicator::Status::Good;
				}
				else {
					return ofxCvGui::Widgets::Indicator::Status::Clear;
				}
			}));
		}


		//----------
		shared_ptr<ofxCvGui::Widgets::Indicator> Widgets::addIndicator(const string & caption, const function<ofxCvGui::Widgets::Indicator::Status()> & get) {
			return this->add(new ofxCvGui::Widgets::Indicator(caption, get));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::LiveValueHistory> Widgets::addLiveValueHistory(const string & caption, const function<float(void)> & get) {
			return this->add(new ofxCvGui::Widgets::LiveValueHistory(caption, get));
		}


		//----------
		shared_ptr<ofxCvGui::Widgets::MultipleChoice> Widgets::addMultipleChoice(const string & caption) {
			return this->add(new ofxCvGui::Widgets::MultipleChoice(caption));
		}


		//----------
		shared_ptr<ofxCvGui::Widgets::MultipleChoice> Widgets::addMultipleChoice(const string & caption, const initializer_list<string> & options) {
			return this->add(new ofxCvGui::Widgets::MultipleChoice(caption, options));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Slider> Widgets::addSlider(ofParameter<float> & parameter) {
			return this->add(new ofxCvGui::Widgets::Slider(parameter));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Spacer> Widgets::addSpacer() {
			return this->add(new ofxCvGui::Widgets::Spacer());
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Title> Widgets::addTitle(const string & caption, ofxCvGui::Widgets::Title::Level level) {
			return this->add(new ofxCvGui::Widgets::Title(caption, level));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Toggle> Widgets::addToggle(ofParameter<bool> & parameter) {
			return this->add(new ofxCvGui::Widgets::Toggle(parameter));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Toggle> Widgets::addToggle(const string & caption, const function<bool()> & get, const function<void(bool)> & set) {
			return this->add(new ofxCvGui::Widgets::Toggle(caption, get, set));
		}

		//----------
		template<typename Type>
		bool tryAddEditableValue(Widgets * panel, shared_ptr<ofAbstractParameter> parameter) {
			auto param = dynamic_pointer_cast<ofParameter<Type>>(parameter);
			if (param) {
				panel->addEditableValue<Type>(*param);
				return true;
			}
			else {
				return false;
			}
		}
		void Widgets::addParameterGroup(ofParameterGroup & parameters, int titleLevel) {
			this->addSpacer();

			if (!parameters.getName().empty()) {
				titleLevel = min(titleLevel, (int)ofxCvGui::Widgets::Title::Level::MaxLevel);
				auto titleLevelTyped = static_cast<ofxCvGui::Widgets::Title::Level>(titleLevel);
				this->addTitle(parameters.getName(), titleLevelTyped);
			}

			for (auto & parameter : parameters) {
				{
					auto param = dynamic_pointer_cast<ofParameter<float>>(parameter);
					if (param) {
						if (ofInRange(param->get(), param->getMin(), param->getMax())) {
							this->addSlider(*param);
						}
						else {
							this->addEditableValue<float>(*param);
						}
						continue;
					}
				}
				{
					auto param = dynamic_pointer_cast<ofParameter<bool>>(parameter);
					if (param) {
						this->addToggle(*param);
						continue;
					}
				}
				if (tryAddEditableValue<uint8_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<uint16_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<uint32_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<uint64_t>(this, parameter)) { continue; }

				if (tryAddEditableValue<int8_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<int16_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<int32_t>(this, parameter)) { continue; }
				if (tryAddEditableValue<int64_t>(this, parameter)) { continue; }

				if (tryAddEditableValue<string>(this, parameter)) { continue; }

				if (tryAddEditableValue<ofVec3f>(this, parameter)) { continue; }

				{
					auto param = dynamic_pointer_cast<ofParameterGroup>(parameter);
					if (param) {
						this->addParameterGroup(*param, titleLevel + 1);
					}
				}
			}
		}

		//----------
		shared_ptr<Panels::Widgets> makeWidgets(string caption) {
			auto newPanel = shared_ptr<Panels::Widgets>(new Panels::Widgets());
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}