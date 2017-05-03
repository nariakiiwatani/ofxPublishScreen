#pragma once

#include "ofMain.h"
#include "ofxZmq.h"
#include "ofxTurboJpeg.h"

namespace ofxPublishScreen {
	
	class Publisher
	{
	public:
		
		Publisher() : thread(NULL) {}
		virtual ~Publisher() { dispose(); }
		
		void setup(int port, int jpeg_quality = 90, int que_max=0);
		void dispose();
		
		void publishScreen();
		void publishPixels(const ofPixels &pix);
		void publishTexture(ofTexture &inputTexture);
		void publishFbo(ofFbo &fbo);
		
		float getFps() const;
		
		int getJpegQuality() const;
		void setJpegQuality(int v);
		
		void onExit(ofEventArgs&);
		
	protected:
		
		class Thread;
		Thread *thread;
		int queMax=0;
	};
	
	class Subscriber
	{
	public:
		
		Subscriber() : thread(NULL) {}
		virtual ~Subscriber() { dispose(); }
		
		void setup(string host, int port);
		void dispose();
		
		void update();

		bool isFrameNew() { return is_frame_new; }
		ofPixelsRef getPixelsRef() { return pix; }
		ofImage& getImage() { return pix; }
		
		float getFps() const;
		
	protected:
		
		ofImage pix;
		bool is_frame_new;
		
		class Thread;
		Thread *thread;
	};
	
	class FboPublisher : public Publisher
	{
	public:
		
		void setup(int port, int w, int h, int internalformat = GL_RGB, int jpeg_quality = 90)
		{
			ofFbo::Settings s = ofFbo::Settings();
			s.width = w;
			s.height = h;
			s.internalformat = internalformat;
			s.useDepth = true;
			fbo.allocate(s);
			
			Publisher::setup(port, jpeg_quality);
		}
		
		void draw(int x = 0, int y = 0)
		{
			fbo.draw(x, y);
		}
		
		void draw(int x, int y, int width, int height)
		{
			fbo.draw(x, y, width, height);
		}
		
		void begin()
		{
			fbo.begin();
			ofFloatColor bg = ofGetCurrentRenderer()->getBackgroundColor();
			ofClear(bg.r * 255, bg.g * 255, bg.b * 255);
		}
		
		void end()
		{
			fbo.end();
			publishFbo(fbo);
		}
		
		float getWidth() const { return fbo.getWidth(); }
		float getHeight() const { return fbo.getHeight(); }
		
		ofFbo& getFbo() { return fbo; }
		
	protected:
		
		ofFbo fbo;
	};

}
