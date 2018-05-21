// CameraPath.h
//

#ifndef CAMERA_PATH_H
#define CAMERA_PATH_H

#include "Interpolator.h"
#include "Camera.h"

struct PathItem
{
	vcg::Point3f eye;
	vcg::Point3f up;
	vcg::Point3f dir;
};

class CameraPath
{
private:

	CatmullRomInterpolator<vcg::Point3f, float> eyeInterp;
	CatmullRomInterpolator<vcg::Point3f, float> upInterp;
	CatmullRomInterpolator<vcg::Point3f, float> dirInterp;

	std::vector<PathItem> keyFrames;

	bool         playing;
	unsigned int pathTime;
	unsigned int startTime;

	void CreatePathInterpolator();


public:
	CameraPath(){playing = false;}

	void play(unsigned int time,unsigned int globalTime);
	void stop();
	void clear();

	void addKeyFrame(Camera *c);
	void use(Camera *c, unsigned int t);

	void load(char *file);
	void save(char *file);

	bool isPlaying()  {return playing;}
	bool isEmpty()    {return keyFrames.size() == 0;}

};



inline void CameraPath::play(unsigned int time,unsigned int globalTime)
{
	CreatePathInterpolator();

	playing    = true;
	pathTime  = time;
	startTime = globalTime;	
}

inline void CameraPath::stop()
{
	playing = false;
}

inline void CameraPath::clear()
{
	playing = false;
	keyFrames.clear();
}


inline void CameraPath::addKeyFrame(Camera *c)
{
	PathItem itm;

	itm.eye = c->getEyePos();
	itm.up  = c->getUp();
	itm.dir = c->getViewDir();

	keyFrames.push_back(itm);
}


inline void CameraPath::CreatePathInterpolator()
{
	size_t sz     = keyFrames.size();
	float *times  = new float [sz];
	
	vcg::Point3f *eyePoints = new vcg::Point3f [sz];
	vcg::Point3f *upPoints  = new vcg::Point3f [sz];
	vcg::Point3f *dirPoints = new vcg::Point3f [sz];

	for (size_t i=0; i<sz; ++i)
	{
		eyePoints[i] = keyFrames[i].eye;
		upPoints[i]  = keyFrames[i].up;
		dirPoints[i] = keyFrames[i].dir;
		times[i]     = (float)i;
	}

	eyeInterp.Set(eyePoints, times, (int)sz, true);
	upInterp.Set(upPoints, times, (int)sz, true);
	dirInterp.Set(dirPoints, times, (int)sz, true);

	delete [] times;
	delete [] eyePoints;
	delete [] upPoints;
	delete [] dirPoints;
}

inline void CameraPath::use(Camera *c, unsigned int currTime)
{
	if (!playing) return;

	const GLuint curr_time   = currTime;
	const float elapsed_time = ((float)(curr_time - startTime)) / 1000.0f;
	const float path_time    = elapsed_time * ((float)(keyFrames.size())) / ((float)pathTime);

	const vcg::Point3f eye = eyeInterp.Interpolate(path_time);
	const vcg::Point3f up = upInterp.Interpolate(path_time);
	const vcg::Point3f viewDir = dirInterp.Interpolate(path_time);

	c->setVectors(eye, up, viewDir);
	
	const int et = curr_time - startTime;


	playing = (elapsed_time <= pathTime);
	
	if (!playing) printf(".....................................\n");
}


inline void CameraPath::load(char *file)
{
	keyFrames.clear();
	FILE * f = fopen(file, "rb");
	size_t sz = 0;
	fread(&sz, sizeof(sz), 1, f);
	keyFrames.resize(sz);
	for (size_t i=0; i<keyFrames.size(); ++i)
	{
		fread(keyFrames[i].eye.V(), sizeof(float), 3, f);
		fread(keyFrames[i].up.V(),  sizeof(float), 3, f);
		fread(keyFrames[i].dir.V(), sizeof(float), 3, f);

	}
	fclose(f);
}

inline void CameraPath::save(char *file)
{
	FILE * f = fopen(file, "wb");
	size_t sz = keyFrames.size();
	fwrite(&sz, sizeof(sz), 1, f);
	for (size_t i=0; i<keyFrames.size(); ++i)
	{
		fwrite(keyFrames[i].eye.V(), sizeof(float), 3, f);
		fwrite(keyFrames[i].up.V(),  sizeof(float), 3, f);
		fwrite(keyFrames[i].dir.V(), sizeof(float), 3, f);

	}
	fclose(f);
}



#endif //CAMERA_PATH_H