/*
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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <OgreSingleton.h>
#include <OgreColourValue.h>

#include <cstdint>

class CreatureDefinition;
class Weapon;
class SpawnCondition;
class CreatureMood;
class Research;
class TileSet;
class TileSetValue;

enum class CreatureMoodLevel;
enum class TileVisual;

//! \brief This class is used to manage global configuration such as network configuration, global creature stats, ...
//! It should NOT be used to load level specific stuff. For that, there is GameMap.
class ConfigManager : public Ogre::Singleton<ConfigManager>
{
public:
    ConfigManager(const std::string& configPath);
    ~ConfigManager();

    static const std::string DefaultWorkerCreatureDefinition;
    static const std::string DEFAULT_TILESET_NAME;

    Ogre::ColourValue getColorFromId(const std::string& id) const;
    inline const std::map<std::string, CreatureDefinition*>& getCreatureDefinitions() const
    { return mCreatureDefs; }
    const CreatureDefinition* getCreatureDefinition(const std::string& name) const;

    inline const std::vector<const Weapon*>& getWeapons() const
    { return mWeapons; }
    const Weapon* getWeapon(const std::string& name) const;

    inline uint32_t getCreatureDeathCounter() const
    { return mCreatureDeathCounter; }

    inline uint32_t getMaxCreaturesPerSeatAbsolute() const
    { return mMaxCreaturesPerSeatAbsolute; }

    inline uint32_t getMaxCreaturesPerSeatDefault() const
    { return mMaxCreaturesPerSeatDefault; }

    inline double getSlapDamagePercent() const
    { return mSlapDamagePercent; }

    inline int64_t getTimePayDay() const
    { return mTimePayDay; }

    inline uint32_t getNetworkPort() const
    { return mNetworkPort; }

    inline uint32_t getClientConnectionTimeout() const
    { return mClientConnectionTimeout; }

    inline uint32_t getBaseSpawnPoint() const
    { return mBaseSpawnPoint; }

    inline const std::map<const std::string, std::vector<const CreatureMood*> >& getCreatureMoodModifiers() const
    { return mCreatureMoodModifiers; }
    CreatureMoodLevel getCreatureMoodLevel(int32_t moodModifiersPoints) const;
    inline int64_t getNbTurnsFuriousMax() const
    { return mNbTurnsFuriousMax; }

    inline double getMaxManaPerSeat() const
    { return mMaxManaPerSeat; }

    inline double getClaimingWallPenalty() const
    { return mClaimingWallPenalty; }

    const std::vector<const SpawnCondition*>& getCreatureSpawnConditions(const CreatureDefinition* def) const;

    //! \brief Get the fighter creature definition spawnable in portals according to the given faction.
    const std::vector<std::string>& getFactionSpawnPool(const std::string& faction) const;

    //! \brief Get the default worker creature definition spawnable according to the given faction.
    const std::string& getFactionWorkerClass(const std::string& faction) const;

    const std::string& getRogueWorkerClass() const
    { return mDefaultWorkerRogue; }

    inline const std::vector<std::string>& getFactions() const
    { return mFactions; }

    //! Rooms configuration
    const std::string& getRoomConfigString(const std::string& param) const;
    uint32_t getRoomConfigUInt32(const std::string& param) const;
    int32_t getRoomConfigInt32(const std::string& param) const;
    double getRoomConfigDouble(const std::string& param) const;

    //! Traps configuration
    const std::string& getTrapConfigString(const std::string& param) const;
    uint32_t getTrapConfigUInt32(const std::string& param) const;
    int32_t getTrapConfigInt32(const std::string& param) const;
    double getTrapConfigDouble(const std::string& param) const;

    //! Spells configuration
    const std::string& getSpellConfigString(const std::string& param) const;
    uint32_t getSpellConfigUInt32(const std::string& param) const;
    int32_t getSpellConfigInt32(const std::string& param) const;
    double getSpellConfigDouble(const std::string& param) const;

    inline const CreatureDefinition* getCreatureDefinitionDefaultWorker() const
    { return mCreatureDefinitionDefaultWorker; }

    inline const std::vector<const Research*>& getResearches() const
    { return mResearches; }

    //! Returns the tileset for the given name. If the tileset is not found, returns the default tileset
    const TileSet* getTileSet(const std::string& tileSetName) const;

private:
    //! \brief Function used to load the global configuration. They should return true if the configuration
    //! is ok and false if a mandatory parameter is missing
    bool loadGlobalConfig(const std::string& configPath);
    bool loadGlobalConfigSeatColors(std::stringstream& configFile);
    bool loadGlobalConfigDefinitionFiles(std::stringstream& configFile);
    bool loadGlobalGameConfig(std::stringstream& configFile);
    bool loadCreatureDefinitions(const std::string& fileName);
    bool loadEquipements(const std::string& fileName);
    bool loadSpawnConditions(const std::string& fileName);
    bool loadFactions(const std::string& fileName);
    bool loadRooms(const std::string& fileName);
    bool loadTraps(const std::string& fileName);
    bool loadSpellConfig(const std::string& fileName);
    bool loadCreaturesMood(const std::string& fileName);
    bool loadResearches(const std::string& fileName);
    bool loadTilesets(const std::string& fileName);
    bool loadTilesetValues(std::istream& defFile, TileVisual tileVisual, std::vector<TileSetValue>& tileValues);

    std::map<std::string, Ogre::ColourValue> mSeatColors;
    std::map<std::string, CreatureDefinition*> mCreatureDefs;
    std::vector<const Weapon*> mWeapons;
    std::string mFilenameCreatureDefinition;
    std::string mFilenameEquipmentDefinition;
    std::string mFilenameSpawnConditions;
    std::string mFilenameFactions;
    std::string mFilenameRooms;
    std::string mFilenameTraps;
    std::string mFilenameSpells;
    std::string mFilenameCreaturesMood;
    std::string mFilenameResearches;
    std::string mFilenameTilesets;
    uint32_t mNetworkPort;
    uint32_t mClientConnectionTimeout;
    uint32_t mBaseSpawnPoint;
    uint32_t mCreatureDeathCounter;
    uint32_t mMaxCreaturesPerSeatAbsolute;
    uint32_t mMaxCreaturesPerSeatDefault;
    int32_t mCreatureBaseMood;
    int32_t mCreatureMoodHappy;
    int32_t mCreatureMoodUpset;
    int32_t mCreatureMoodAngry;
    int32_t mCreatureMoodFurious;
    double mSlapDamagePercent;
    int64_t mTimePayDay;
    int64_t mNbTurnsFuriousMax;
    double mMaxManaPerSeat;
    double mClaimingWallPenalty;
    std::string mDefaultWorkerRogue;
    std::map<const CreatureDefinition*, std::vector<const SpawnCondition*> > mCreatureSpawnConditions;
    std::map<const std::string, std::vector<const CreatureMood*> > mCreatureMoodModifiers;
    std::map<const std::string, std::vector<std::string> > mFactionSpawnPool;

    //! \brief Stores the faction default worker creature definition.
    std::map<const std::string, std::string> mFactionDefaultWorkerClass;

    std::vector<std::string> mFactions;
    std::map<const std::string, std::string> mRoomsConfig;
    std::map<const std::string, std::string> mTrapsConfig;
    std::map<const std::string, std::string> mSpellConfig;

    //! Default definition for the editor. At map loading, it will spawn a creature from
    //! the default seat worker depending on seat faction
    CreatureDefinition* mCreatureDefinitionDefaultWorker;

    //! Allowed researches
    std::vector<const Research*> mResearches;

    //! Allowed tilesets
    std::map<std::string, const TileSet*> mTileSets;
};

#endif //CONFIGMANAGER_H
