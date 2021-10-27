#include "pch_ofxCvGui.h"

//----------
OFXSINGLETON_DEFINE(ofxCvGui::InspectController);

namespace ofxCvGui {
#pragma mark IInspectable
	//----------
	bool IInspectable::isBeingInspected() const {
		return InspectController::X().getTarget().get() == this;
	}

#pragma mark InspectController
	//----------
	InspectController::InspectController() {
		this->onClear += [this](InspectArguments& args) {
			auto inspector = args.inspector;
			if (this->getHistorySize() > 0) {
				inspector->addButton("<< Go back", [this]() {
					this->back();
					});
				inspector->addSpacer();
			}
		};
	}

	//----------
	void InspectController::update() {
		bool needsToNotifyListeners = false;

		//if we got a command to go back then service it
		if (this->goBackThisFrame) {
			bool foundValidInspectableInHistory = false;

			if (!this->history.empty()) {
				auto it = this->history.rbegin();

				for (; it != this->history.rend(); it++) {
					auto inspectable = it->lock();

					// Only inspect if it's a valid pointer
					if (inspectable) {
						// erase everything after the highest valid
						this->history.erase(std::next(it).base(), this->history.end());
						this->currentTarget = inspectable;
						needsToNotifyListeners = true;
						foundValidInspectableInHistory = true;
						break;
					}
				}
			}

			if (!foundValidInspectableInHistory) {
				this->history.clear();
				this->clearThisFrame = true;
			}
			this->goBackThisFrame = false;
		}


		//if this frame we got a command to clear
		if (this->clearThisFrame) {
			this->currentTarget.reset();
			needsToNotifyListeners = true;
			this->clearThisFrame = false;
		}

		//if a selection was made this frame then take it
		auto inspectThisFrame = this->inspectThisFrame.lock();
		if (inspectThisFrame) {
			if (this->currentTarget.lock() != inspectThisFrame) {
				if (this->currentTarget.lock()) {
					this->history.push_back(this->currentTarget);
				}
				this->currentTarget = this->inspectThisFrame;
				needsToNotifyListeners = true;
			}
			this->inspectThisFrame.reset();
		}

		//trim history
		if (this->history.size() > this->historyMaxSize) {
			this->history.erase(this->history.begin()
				, this->history.begin() + (this->history.size() - this->historyMaxSize));
		}

		//if a refresh is activated
		if (this->refreshThisFrame) {
			if (this->currentTarget.lock()) {
				//if we have a valid target
				needsToNotifyListeners = true;
			}
			this->refreshThisFrame = false;
		}

		//if our target has been deleted elsewhere, let's drop our attention to it before something weird happens
		if (this->currentTarget.expired() && this->hasTarget) {
			needsToNotifyListeners = true;
		}

		//notify inspectors of any change in target
		if (needsToNotifyListeners) {
			auto target = this->currentTarget.lock();
			this->hasTarget = (bool)target;
			this->onTargetChange(target);
		}
	}

	//----------
	void InspectController::inspect(shared_ptr<IInspectable> target) {
		if (!this->inspectorLocked) {
			//only set if nothing else has been set this frame
			if (!this->inspectThisFrame.lock()) {
				if (target) {
					this->inspectThisFrame = target;
				}
				else {
					this->clear();
				}
			}
		}
	}

	//----------
	void InspectController::back() {
		this->goBackThisFrame = true;
	}

	//----------
	void InspectController::refresh() {
		this->refreshThisFrame = true;
	}

	//----------
	void InspectController::refresh(IInspectable* target) {
		if (this->currentTarget.lock().get() == target) {
			this->refresh();
		}
	}

	//----------
	void InspectController::clear() {
		this->clearThisFrame = true;
	}

	//----------
	shared_ptr<IInspectable> InspectController::getTarget() const {
		return this->currentTarget.lock();
	}

	//----------
	void InspectController::addToInspector(ElementPtr element) {
		this->onAddWidget.notifyListeners(element);
	}

	//----------
	void InspectController::maximise() {
		this->onMaximise.notifyListeners();
	}

	//----------
	void InspectController::setInspectorLocked(bool inspectorLocked) {
		this->inspectorLocked = inspectorLocked;
	}

	//----------
	bool InspectController::getInspectorLocked() const {
		return this->inspectorLocked;
	}

	//----------
	size_t InspectController::getHistorySize() const {
		return this->history.size();
	}

	//----------
	size_t InspectController::getHistoryMaxSize() const {
		return this->historyMaxSize;
	}

	//----------
	void InspectController::setHistoryMaxSize(size_t value) {
		this->historyMaxSize = value;
	}

#pragma mark Global
	//-----------
	void inspect(shared_ptr<IInspectable> target) {
		InspectController::X().inspect(target);
	}

	//-----------
	bool isBeingInspected(shared_ptr<IInspectable> target) {
		return target == InspectController::X().getTarget();
	}

	//-----------
	bool isBeingInspected(IInspectable & target) {
		return isBeingInspected(&target);
	}

	//-----------
	bool isBeingInspected(IInspectable * target) {
		auto currentInspectTarget = InspectController::X().getTarget();
		return target == currentInspectTarget.get();
	}

	//-----------
	void addToInspector(ofxCvGui::ElementPtr element) {
		InspectController::X().addToInspector(element);
	}

	//-----------
	void refreshInspector(IInspectable * target) {
		InspectController::X().refresh(target);
	}
}