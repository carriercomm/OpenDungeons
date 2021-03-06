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

#include "entities/CraftedTrap.h"

#include "entities/Creature.h"
#include "entities/Tile.h"

#include "network/ODPacket.h"

#include "gamemap/GameMap.h"

#include "traps/Trap.h"
#include "traps/TrapBoulder.h"
#include "traps/TrapCannon.h"
#include "traps/TrapDoor.h"
#include "traps/TrapSpike.h"

#include "utils/Helper.h"
#include "utils/LogManager.h"
#include "utils/Random.h"

#include <iostream>

const std::string EMPTY_STRING;

const Ogre::Vector3 SCALE(0.5,0.5,0.5);

CraftedTrap::CraftedTrap(GameMap* gameMap, bool isOnServerMap, const std::string& workshopName, TrapType trapType) :
    RenderedMovableEntity(gameMap, isOnServerMap, workshopName, getMeshFromTrapType(trapType), 0.0f, false),
    mTrapType(trapType)
{
}

CraftedTrap::CraftedTrap(GameMap* gameMap, bool isOnServerMap) :
    RenderedMovableEntity(gameMap, isOnServerMap)
{
}

const Ogre::Vector3& CraftedTrap::getScale() const
{
    return SCALE;
}

const std::string& CraftedTrap::getMeshFromTrapType(TrapType trapType)
{
    // TODO: let the TrapManager handle that
    switch(trapType)
    {
        case TrapType::nullTrapType:
            return EMPTY_STRING;
        case TrapType::cannon:
            return TrapCannon::MESH_CANON;
        case TrapType::spike:
            return TrapSpike::MESH_SPIKE;
        case TrapType::boulder:
            return TrapBoulder::MESH_BOULDER;
        case TrapType::doorWooden:
            return TrapDoor::MESH_DOOR;
        default:
            OD_ASSERT_TRUE_MSG(false, "Wrong enum asked for CraftedTrap " + getName() + ", trapType="
                + Helper::toString(static_cast<uint32_t>(trapType)));
    }

    return EMPTY_STRING;
}

void CraftedTrap::notifyEntityCarryOn(Creature* carrier)
{
    Tile* myTile = getPositionTile();
    OD_ASSERT_TRUE_MSG(myTile != nullptr, "name=" + getName());
    if(myTile == nullptr)
        return;

    setIsOnMap(false);
    setSeat(carrier->getSeat());
    myTile->removeEntity(this);
}

void CraftedTrap::notifyEntityCarryOff(const Ogre::Vector3& position)
{
    mPosition = position;
    setIsOnMap(true);

    Tile* myTile = getPositionTile();
    OD_ASSERT_TRUE_MSG(myTile != nullptr, "name=" + getName());
    if(myTile == nullptr)
        return;

    myTile->addEntity(this);
}

CraftedTrap* CraftedTrap::getCraftedTrapFromStream(GameMap* gameMap, std::istream& is)
{
    CraftedTrap* obj = new CraftedTrap(gameMap, true);
    return obj;
}

CraftedTrap* CraftedTrap::getCraftedTrapFromPacket(GameMap* gameMap, ODPacket& is)
{
    CraftedTrap* obj = new CraftedTrap(gameMap, false);
    return obj;
}

void CraftedTrap::exportToStream(std::ostream& os) const
{
    RenderedMovableEntity::exportToStream(os);
    os << mTrapType << mPosition.x << "\t" << mPosition.y << "\t" << mPosition.z << "\t";
}

void CraftedTrap::importFromStream(std::istream& is)
{
    RenderedMovableEntity::importFromStream(is);
    OD_ASSERT_TRUE(is >> mTrapType >> mPosition.x >> mPosition.y >> mPosition.z);
}

std::string CraftedTrap::getCraftedTrapStreamFormat()
{
    std::string format = RenderedMovableEntity::getRenderedMovableEntityStreamFormat();
    if(!format.empty())
        format += "\t";

    format += "trapType\tPosX\tPosY\tPosZ";

    return format;
}
