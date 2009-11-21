/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//osgParticle - Copyright (C) 2002 Marco Jez

#ifndef OSGPARTICLE_MODULARPROGRAM_
#define OSGPARTICLE_MODULARPROGRAM_ 1

#include "Export.h"
#include "Program.h"
#include "Operator.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>

namespace osgParticleHPS
{

    /**    A program class for performing operations on particles using a sequence of <I>operators</I>.
        To use a <CODE>ModularProgram</CODE> you have to create some <CODE>Operator</CODE> objects and 
        add them to the program.
        All operators will be applied to each particle in the same order they've been added to the program.
    */    
    class OSGPARTICLE_EXPORT ModularProgram: public Program {
    public:
        ModularProgram();
        ModularProgram(const ModularProgram &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Node(osgParticleHPS,ModularProgram);
        
        /// Get the number of operators.
        inline int numOperators() const;
        
        /// Add an operator to the list.        
        inline void addOperator(Operator *o);
        
        /// Get a pointer to an operator in the list.
        inline Operator *getOperator(int i);
        
        /// Get a const pointer to an operator in the list.
        inline const Operator *getOperator(int i) const;
        
        /// Remove an operator from the list.
        inline void removeOperator(int i);        
            
    protected:
        virtual ~ModularProgram() {}
        ModularProgram &operator=(const ModularProgram &) { return *this; }
        
        void execute(double dt);
        
    private:
        typedef std::vector<osg::ref_ptr<Operator> > Operator_vector;

        Operator_vector operators_;
    };
    
    // INLINE FUNCTIONS
    
    inline int ModularProgram::numOperators() const
    {
        return static_cast<int>(operators_.size());
    }

    inline void ModularProgram::addOperator(Operator *o)
    {
        operators_.push_back(o);
    }

    inline Operator *ModularProgram::getOperator(int i)
    {
        return operators_[i].get();
    }

    inline const Operator *ModularProgram::getOperator(int i) const
    {
        return operators_[i].get();
    }

    inline void ModularProgram::removeOperator(int i)
    {
        operators_.erase(operators_.begin()+i);
    }
    

}

#endif
