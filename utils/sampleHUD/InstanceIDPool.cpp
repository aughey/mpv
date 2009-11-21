/*
 *  acm : an aerial combat simulator for X
 *  Copyright (C) 2007       Andrew Sampson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 dated June, 1991.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;  if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave., Cambridge, MA 02139, USA.
 */

#include "InstanceIDPool.h"

InstanceIDPool::InstanceIDPool()
{
	nextFreeID = 0;
	
	// build up a nice initial list of IDs
	for( int i = 0; i < 64; i++ )
	{
		addNewIDToPool();
	}
}


InstanceIDPool::~InstanceIDPool()
{
}


int InstanceIDPool::getAvailableID()
{
	int result = -1;
	
	if( pool.empty() )
		addNewIDToPool();
	
	// fixme - assert pool not empty
	
	result = pool.front();
	pool.pop();
	
	return result;
}


void InstanceIDPool::relenquishID( int id )
{
	pool.push( id );
}


void InstanceIDPool::addNewIDToPool()
{
	pool.push( nextFreeID );
	nextFreeID++;
}
