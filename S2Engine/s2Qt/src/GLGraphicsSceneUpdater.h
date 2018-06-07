// GLGraphicsSceneUpdater.h
// 
#ifndef GUITOOLS_GLGRAPHICSSCENEUPDATER_H
#define GUITOOLS_GLGRAPHICSSCENEUPDATER_H

#include <QThread>
#include <QTimer>

namespace s2 {

namespace Qt {


class GLGraphicsScene;

class GLGraphicsSceneUpdater: public QObject// : public QThread
{
	Q_OBJECT

	//friend class GLGraphicsScene;

public:
	GLGraphicsSceneUpdater( GLGraphicsScene *scene, unsigned int msec );
	virtual ~GLGraphicsSceneUpdater();

	void setRefreshTime( unsigned int msec );
	unsigned int refreshTime() const;

	void start();
	void stop();	

private slots:
	void run();// override;

private:
	unsigned int    _refreshTime;   //In ms
	bool            _running;
    QTimer          _timer;
    GLGraphicsScene *_scene;
};


}}


#endif