#include <debug_tools.hpp>
#include <sound_manager.hpp>
#include <fmod.hpp>

jeBegin

FMOD::System* SoundManager::system_ = nullptr;

void SoundManager::init_fmod()
{
    // create fmod system
    FMOD_RESULT result;
    result = FMOD::System_Create(&system_);
    DEBUG_ASSERT(FMOD_RESULT::FMOD_OK != result, "Failed to init FMOD");
    
    // check version
    unsigned int version;
    result = system_->getVersion(&version);
    DEBUG_ASSERT(FMOD_RESULT::FMOD_OK != result, "Version error of FMOD");
    if (version < FMOD_VERSION)
        jeDebugPrint("FMOD version error or FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

    // init fmod system
    void* extradriverdata = nullptr;
    system_->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

void SoundManager::close_fmod()
{
    system_->close();
    system_->release();
}

jeEnd