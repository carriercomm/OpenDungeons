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

#ifndef ROOMLIBRARY_H
#define ROOMLIBRARY_H

#include "rooms/Room.h"
#include "rooms/RoomType.h"

class Tile;

enum class ResearchType;

class RoomLibraryTileData : public TileData
{
public:
    RoomLibraryTileData() :
        TileData(),
        mCanHaveResearchEntity(true)
    {}

    RoomLibraryTileData(const RoomLibraryTileData* roomLibraryTileData) :
        TileData(roomLibraryTileData),
        mCanHaveResearchEntity(roomLibraryTileData->mCanHaveResearchEntity)
    {}

    virtual ~RoomLibraryTileData()
    {}

    virtual RoomLibraryTileData* cloneTileData() const override
    { return new RoomLibraryTileData(this); }

    bool mCanHaveResearchEntity;
};

class RoomLibrary: public Room
{
public:
    RoomLibrary(GameMap* gameMap);

    virtual RoomType getType() const override
    { return RoomType::library; }

    virtual void doUpkeep() override;
    virtual bool hasOpenCreatureSpot(Creature* c) override;
    virtual bool addCreatureUsingRoom(Creature* c) override;
    virtual void removeCreatureUsingRoom(Creature* c) override;
    virtual void absorbRoom(Room *r) override;

    static int getRoomCost(std::vector<Tile*>& tiles, GameMap* gameMap, RoomType type,
        int tileX1, int tileY1, int tileX2, int tileY2, Player* player);
    static void buildRoom(GameMap* gameMap, const std::vector<Tile*>& tiles, Seat* seat);
    static Room* getRoomFromStream(GameMap* gameMap, std::istream& is);

protected:
    RoomLibraryTileData* createTileData(Tile* tile) override;
    virtual RenderedMovableEntity* notifyActiveSpotCreated(ActiveSpotPlace place, Tile* tile) override;
    virtual void notifyActiveSpotRemoved(ActiveSpotPlace place, Tile* tile) override;
private:
    //!\brief checks how many items are on the library
    uint32_t countResearchItemsOnRoom();
    Tile* checkIfAvailableSpot();
    void getCreatureWantedPos(Creature* creature, Tile* tileSpot,
        Ogre::Real& wantedX, Ogre::Real& wantedY);
    std::vector<Tile*> mUnusedSpots;
    std::map<Creature*,Tile*> mCreaturesSpots;
};

#endif // ROOMLIBRARY_H
