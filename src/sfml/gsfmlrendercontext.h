#ifndef GSFMLRENDERCONTEXT_H
#define GSFMLRENDERCONTEXT_H

#include "gincu/grendercontext.h"

#include "gsfmlutil.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace gincu {

enum class GSfmlRenderCommandType
{
	none,
	image,
	text,
	vertexArray,
	switchCamera
};

class GTextureData;
class GTextRenderData;
class GCameraData;
class GVertexArrayData;
enum class GPrimitive;

struct GVertexCommand
{
	std::shared_ptr<GVertexArrayData> vertexArrayData;
	GPrimitive primitive;
	std::shared_ptr<GTextureData> textureData;
};

struct GSfmlRenderCommand
{
	GSfmlRenderCommand();
	GSfmlRenderCommand(const std::shared_ptr<GCameraData> & cameraData);
	GSfmlRenderCommand(const std::shared_ptr<GTextureData> & textureData, const GRect & rect, const GMatrix44 & matrix, const GRenderInfo * renderInfo);
	GSfmlRenderCommand(const std::shared_ptr<GTextRenderData> & textData, const GMatrix44 & matrix, const GRenderInfo * renderInfo);
	GSfmlRenderCommand(const std::shared_ptr<GVertexCommand> & vertexCommand, const GMatrix44 & matrix, const GRenderInfo * renderInfo);
	
	void doCopyRenderInfo(const GRenderInfo * renderInfo);

	GSfmlRenderCommandType type;

	std::shared_ptr<void> renderData;

	GRect rect;
	sf::RenderStates sfmlRenderStates;
	GColor color;
};

class GRenderEngineLock
{
public:
	GRenderEngineLock() : value(false) {}

	void set() {
		this->value = true;
		this->signal.notify_one();
	}

	void reset() {
		this->value = false;
	}

	void wait() {
		std::unique_lock<std::mutex> lock(this->mutex);
		this->signal.wait(lock, [=]() { return (bool)this->value; });
	}

private:
	std::atomic_bool value;
	std::mutex mutex;
	std::condition_variable signal;
};

class GSfmlRenderContext : public GRenderContext
{
private:
	typedef std::vector<GSfmlRenderCommand> RenderCommandQueue;

public:
	GSfmlRenderContext();
	~GSfmlRenderContext();

	void initialize(sf::RenderWindow * window);
	void finalize();

private:
	void threadMain();
	void processRenderCommands();
	void batchDrawImages(const int firstIndex, const int lastIndex);

private:
	virtual void setBackgroundColor(const GColor color);

	virtual void render(const cpgf::GCallback<void (GRenderContext *)> & renderCallback) override;

	virtual void switchCamera(const GCamera & camera) override;

	virtual void draw(
		const GTexture & texture,
		const GRect & rect,
		const GMatrix44 & matrix,
		const GRenderInfo * renderInfo
	) override;

	virtual void draw(
		const GTextRender & text,
		const GMatrix44 & matrix,
		const GRenderInfo * renderInfo
	) override;

	virtual void draw(
		const GVertexArray & vertexArray,
		const GPrimitive type,
		const GTexture & texture,
		const GMatrix44 & matrix,
		const GRenderInfo * renderInfo
	) override;
	
private:
	GColor backgroundColor;
	sf::RenderWindow * window;

	std::atomic_bool finished;
	GRenderEngineLock updaterReadyLock;
	RenderCommandQueue queueStorage[2];
	RenderCommandQueue * updaterQueue;
	RenderCommandQueue * renderQueue;
	std::mutex updaterQueueMutex;
	std::vector<RenderCommandQueue> commandQueueDeleter;
};


} //namespace gincu


#endif

