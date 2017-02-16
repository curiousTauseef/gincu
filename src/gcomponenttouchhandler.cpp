#include "gincu/gcomponenttouchhandler.h"
#include "gincu/gcomponentrender.h"
#include "gincu/gcomponenttransform.h"
#include "gincu/gentity.h"
#include "gincu/gentityutil.h"
#include "gincu/gutil.h"
#include "gincu/grenderanchor.h"

namespace gincu {

GComponentTouchHandler::GComponentTouchHandler()
	:
		super(this),
		onTouchedList(std::make_shared<OnTouchedList>())
{
}

GComponentTouchHandler * GComponentTouchHandler::addOnTouch(const GComponentTouchHandler::OnTouched & onTouch)
{
	this->onTouchedList->add(onTouch);
	
	return this;
}

void GComponentTouchHandler::removeOnTouch(const GComponentTouchHandler::OnTouched & onTouch)
{
	this->onTouchedList->remove(onTouch);
}

bool GComponentTouchHandler::canHandle(const GPoint & worldPosition) const
{
	return doCanHandle(worldPosition);
}

void GComponentTouchHandler::handle(const GEvent & touchEvent)
{
	// The touch handler may be freed during event handling,
	// so we have to cache the callback list in a local variable.
	auto tempOnTouchedList = this->onTouchedList;
	(*tempOnTouchedList)(touchEvent);
}


GComponentRendererTouchHandler::GComponentRendererTouchHandler()
{
}

bool GComponentRendererTouchHandler::doCanHandle(const GPoint & worldPosition) const
{
	GComponentTransform * transform = getEntity()->getComponentByType<GComponentTransform>();
	if(! transform->isVisible()) {
		return false;
	}

	const GMatrix44 matrix = computeRenderableMatrix(transform);
	const GSize size = getEntity()->getComponentByType<GComponentRender>()->getSize();

	const GPoint normalizedPoint = transformPoint(inverseMatrix(matrix), { worldPosition.x, worldPosition.y });
	return isInRect(normalizedPoint, createRect(GPoint(), size));
}


} //namespace gincu
