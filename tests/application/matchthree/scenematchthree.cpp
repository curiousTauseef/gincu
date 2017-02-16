#include "matchthree/scenematchthree.h"
#include "matchthree/matchthreeboard.h"
#include "matchthree/matchthreeinfoview.h"
#include "matchthree/matchthreechess.h"
#include "matchthree/matchthreeconsts.h"
#include "matchthree/matchthreestates.h"
#include "matchthree/componentchess.h"
#include "scenemenu.h"
#include "uiutil.h"
#include "gincu/gstatemachine.h"
#include "gincu/gentity.h"
#include "gincu/gcomponentrender.h"
#include "gincu/gcomponenttransform.h"
#include "gincu/gcomponenttouchhandler.h"
#include "gincu/gscenemanager.h"
#include "gincu/gutil.h"
#include "gincu/gapplication.h"
#include "cpgf/tween/gtweenlist.h"
#include "cpgf/goutmain.h"

#include "matchthree/stateinitializenewround.h"
#include "matchthree/statecheckdeadboard.h"
#include "matchthree/statemainloop.h"
#include "matchthree/stateswapchesses.h"
#include "matchthree/stateerasematches.h"
#include "matchthree/stateresult.h"

#include <cmath>

namespace gincu {

SceneMatchThree::SceneMatchThree()
{
}

SceneMatchThree::~SceneMatchThree()
{
}

void SceneMatchThree::doOnEnter()
{
	this->addEntity(
		(new GEntity())
			->addComponent(createComponent<GComponentTransform>())
			->addComponent(createAndLoadImageComponent(backgroundImageName))
	);
	this->addEntity(createBackButton(cpgf::makeCallback(this, &SceneMatchThree::onQuitGameClicked)));

	this->stateMachine.reset(new GStateMachine());
	this->board.reset(new MatchThreeBoard(this));
	this->infoView.reset(new MatchThreeInfoView(this));

	this->stateMachine->registerState(new StateInitializeNewRound(this));
	this->stateMachine->registerState(new StateCheckDeadBoard(this));
	this->stateMachine->registerState(new StateMainLoop(this));
	this->stateMachine->registerState(new StateSwapChesses(this, stateSwapChesses, stateEraseMatches));
	this->stateMachine->registerState(new StateSwapChesses(this, stateSwapBackChesses, stateMainLoop));
	this->stateMachine->registerState(new StateEraseMatches(this, stateEraseMatches, stateSwapBackChesses));
	this->stateMachine->registerState(new StateEraseMatches(this, stateCheckEraseMatches, stateCheckDeadBoard));
	this->stateMachine->registerState(new StateResult(this));

	this->stateMachine->gotoState(stateInitializeNewRound);

	this->roundStartMilliseconds = getMilliseconds();

	GApplication::getInstance()->addUpdater(cpgf::makeCallback(this, &SceneMatchThree::onUpdate));

	this->infoView->setRemainingSeconds(secondsPerRound);

	this->previousTouchPosition.x = -1;
}

void SceneMatchThree::doOnExit()
{
	GApplication::getInstance()->removeUpdater(cpgf::makeCallback(this, &SceneMatchThree::onUpdate));
}

void SceneMatchThree::clearTouchedChessList()
{
	this->touchedChessList.clear();
}

void SceneMatchThree::restoreTouchedChessList()
{
	for(auto it = this->touchedChessList.begin(); it != this->touchedChessList.end(); ++it) {
		(*it)->getComponentByType<GComponentTransform>()->setScale(GScale { 1.0f, 1.0f });
	}
}

void SceneMatchThree::onUpdate()
{
	this->infoView->setRemainingSeconds(secondsPerRound - (getMilliseconds() - this->roundStartMilliseconds) / 1000);

	if(this->isTimeUp()) {
		if(this->stateMachine->getCurrentStateId() == stateMainLoop) {
			this->stateMachine->gotoState(stateResult);
		}
	}
}

void SceneMatchThree::onQuitGameClicked()
{
	this->roundStartMilliseconds = getMilliseconds() - secondsPerRound * 1000;
}

void SceneMatchThree::onChessTouched(const GEvent & touchEvent)
{
	if(this->stateMachine->getCurrentStateId() != stateMainLoop || isTimeUp()) {
		return;
	}

	switch(touchEvent.type) {
	case GEventType::touchPressed:
		if(touchEvent.touch.touchedEntity == nullptr || touchEvent.touch.touchedEntity->getComponentByType<ComponentChess>() == nullptr) {
			this->restoreTouchedChessList();
			this->clearTouchedChessList();
		}
		else {
			if(this->touchedChessList.size() == 1) {
				GEntity * chessA = touchedChessList[0];
				RowColumn cellA = board->getChessCell(chessA);
				RowColumn cellB = board->getChessCell(touchEvent.touch.touchedEntity);

				if(chessA == touchEvent.touch.touchedEntity || ! areCellsNeighbors(cellA, cellB)) {
					this->restoreTouchedChessList();
					this->clearTouchedChessList();
				}

				if(chessA == touchEvent.touch.touchedEntity) {
					break;
				}
			}
			if(this->touchedChessList.empty()) {
				touchEvent.touch.touchedEntity->getComponentByType<GComponentTransform>()->setScale(GScale { 1.2f, 1.2f });
			}

			this->touchedChessList.push_back(touchEvent.touch.touchedEntity);
			this->setTouchCapture(touchEvent.touch.touchedEntity);

			this->previousTouchPosition = touchEvent.touch.worldPosition;
		}
		break;

	case GEventType::touchReleased:
		this->previousTouchPosition.x = -1;
		if(touchEvent.touch.touchedEntity == nullptr || touchEvent.touch.touchedEntity->getComponentByType<ComponentChess>() == nullptr) {
			this->restoreTouchedChessList();
			this->clearTouchedChessList();
		}
		else {
			if(! this->touchedChessList.empty()) {
				if(this->touchedChessList.back() != touchEvent.touch.touchedEntity) {
					this->touchedChessList.push_back(touchEvent.touch.touchedEntity);
					this->setTouchCapture(nullptr);
				}
			}
		}
		break;

	case GEventType::touchMoved:
		if(this->previousTouchPosition.x > 0
			&& this->touchedChessList.size() == 1
			&& this->touchedChessList.back() != touchEvent.touch.touchedEntity
			) {
			const RowColumn cell = this->board->getChessCell(this->touchedChessList.back());
			const GCoord deltaX = fabs(touchEvent.touch.worldPosition.x - this->previousTouchPosition.x);
			const GCoord deltaY = fabs(touchEvent.touch.worldPosition.y - this->previousTouchPosition.y);
			RowColumn cellToSwap = cell;
			if(deltaX > deltaY) {
				if(touchEvent.touch.worldPosition.x > this->previousTouchPosition.x) {
					++cellToSwap.column;
				}
				else {
					--cellToSwap.column;
				}
			}
			else {
				if(touchEvent.touch.worldPosition.y > this->previousTouchPosition.y) {
					++cellToSwap.row;
				}
				else {
					--cellToSwap.row;
				}
			}

			GEntity * chessToSwap = this->board->getChessAt(cellToSwap);
			if(chessToSwap != nullptr) {
				this->touchedChessList.push_back(chessToSwap);
			}
		}
		break;
		
	default:
		break;
	}

	if(this->touchedChessList.size() == 2) {
		this->previousTouchPosition.x = -1;
		this->setTouchCapture(nullptr);
		this->restoreTouchedChessList();
		this->stateMachine->gotoState(stateSwapChesses);
	}
}

void SceneMatchThree::gotScore(const int score)
{
	this->infoView->addScore(score);
}

int SceneMatchThree::getTotalScore() const
{
	return this->infoView->getTotalScore();
}

bool SceneMatchThree::isTimeUp() const
{
	return this->infoView->getRemainingSeconds() <= 0;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	MenuRegister::getInstance()->registerItem("match three", 1, [](){
			GApplication::getInstance()->getSceneManager()->switchScene(new SceneMatchThree());
		},
		0xffeeee77);
}


} //namespace gincu
