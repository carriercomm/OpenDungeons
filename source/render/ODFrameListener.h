/*!
 * \file   ODFrameListener.h
 * \date   09 April 2011
 * \auth	(require 'ecb)or Ogre team, andrewbuck, oln, StefanP.MUC
 * \brief  Handles the input and rendering request
 *
 *  Copyright (C) 2011-2015  OpenDungeons Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ODFRAMELISTENER_H__
#define __ODFRAMELISTENER_H__

#include "camera/CameraManager.h"
#include "utils/FrameRateLimiter.h"

#include <OgreFrameListener.h>
#include <OgreSceneQuery.h>
#include <OgreRenderQueueListener.h>
#include <OgreSingleton.h>
#include <OgreWindowEventUtilities.h>

#include <deque>
#include <memory>

class ChatMessage;
class GameMap;
class Gui;
class ModeManager;
class RenderManager;

namespace CEGUI
{
    class EventArgs;
}

namespace OIS
{
    class MouseEvent;
}

namespace Ogre
{
    class OverlaySystem;
}

/*! \brief The main OGRE rendering class.
 *
 * This class provides the rendering framework for the OGRE subsystem, as well
 * as processing keyboard and mouse input from the user.  It loads and
 * initializes the meshes for creatures and tiles, moves the camera, and
 * displays the terminal and chat messages on the game screen.
 */
class ODFrameListener :
        public Ogre::Singleton<ODFrameListener>,
        public Ogre::FrameListener,
        public Ogre::WindowEventListener,
        public Ogre::RenderQueueListener
{

friend class ODClient;

public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    ODFrameListener(Ogre::RenderWindow* renderWindow, Ogre::OverlaySystem* overLaySystem, Gui* gui);
    virtual ~ODFrameListener();

    void requestExit();

    inline float getEventMaxTimeDisplay() const
    { return mEventMaxTimeDisplay; }

    inline void setEventMaxTimeDisplay(float eventMaxTimeDisplay)
    { mEventMaxTimeDisplay = eventMaxTimeDisplay; }

    //! \brief Toggle the display of debug info (default key used to do that: F11)
    void toggleDebugInfo()
    { mShowDebugInfo = !mShowDebugInfo; }

    //! \brief Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw) override;

    /*! \brief The main function for the OGRE 3d environment.
     *
     * This function is triggered by Ogre 3D once all the rendering has been bound to GPU,
     * giving time to the CPU to handle updates. Hence, we're placing our update logic here
     * to use the CPU while it's less busy and earn performance.
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;

    //! \brief Triggered once a frame rendering has ended.
    bool frameEnded(const Ogre::FrameEvent& evt) override;

    //! \brief From Ogre::RenderQueueListener
    void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
        bool& skipThisInvocation) override;

    //! \brief Exit the game.
    bool quit(const CEGUI::EventArgs &e);

    //! \brief Setup the ray scene query, use CEGUI's mouse position
    //! This permits to get the mouse world coordinates and other entities present below it.
    Ogre::RaySceneQueryResult& doRaySceneQuery(const OIS::MouseEvent &arg);

    //! \brief Setup the ray scene query, use CEGUI's mouse position
    //! This permits to get the mouse world coordinates and other entities present below it.
    //! Also sets keeperHand3DPos to the 3D world position the cursor is on
    Ogre::RaySceneQueryResult& doRaySceneQuery(const OIS::MouseEvent &arg, Ogre::Vector3& keeperHand3DPos);

    /*! \brief Print a string in the upper right corner of the screen.
     *  Usually used for system or debug info
     */
    void printDebugInfo();

    inline GameMap* getClientGameMap()
    { return mGameMap.get(); }

    inline const GameMap* getClientGameMap() const
    { return mGameMap.get(); }

    inline ModeManager* getModeManager()
    { return mModeManager.get(); }

    inline Ogre::RenderWindow* getRenderWindow()
    { return mWindow; }

    //! \brief Accessors for camera manager
    void setCameraPosition(const Ogre::Vector3& position);
    void moveCamera(CameraManager::Direction direction);

    void setActiveCameraNearClipDistance(Ogre::Real value);
    Ogre::Real getActiveCameraNearClipDistance();
    void setActiveCameraFarClipDistance(Ogre::Real value);
    Ogre::Real getActiveCameraFarClipDistance();
    Ogre::Vector3 getCameraViewTarget();
    void cameraFlyTo(const Ogre::Vector3& destination);

    CameraManager* getCameraManager()
    {
        return &mCameraManager;
    }

    //! \brief Set max framerate
    inline void setMaxFPS(unsigned int fps)
    {
        mFpsLimiter.setFrameRate(fps);
    }

    inline unsigned int getMaxFPS()
    {
        return mFpsLimiter.getFrameRate();
    }

private:
    //! \brief Tells whether the frame listener is initialized.
    bool mInitialized;

    //! \brief The Ogre render window reference. Don't delete it.
    Ogre::RenderWindow* mWindow;

    //! \brief Foreign reference to gui.
    Gui*                 mGui;
    std::unique_ptr<RenderManager> mRenderManager;
    std::unique_ptr<GameMap>       mGameMap;
    std::unique_ptr<ModeManager>   mModeManager;

    bool                 mShowDebugInfo;
    bool                 mContinue;

    //! \brief The amount of time in seconds an event message will be displayed.
    float                mEventMaxTimeDisplay;

    //! \brief Reference to the Ogre ray scene query handler. Don't delete it.
    Ogre::RaySceneQuery* mRaySceneQuery;

    //! \brief To see if the frameListener wants to exit
    bool mExitRequested;

    //! \brief The Camera manager
    CameraManager mCameraManager;

    FrameRateLimiter mFpsLimiter;

    //! \brief Actually exit application
    void exitApplication();

    //! \brief Updates server-turn independent creature animation, audio, and overall rendering.
    void updateAnimations(Ogre::Real timeSinceLastFrame);
};

#endif // __ODFRAMELISTENER_H__
