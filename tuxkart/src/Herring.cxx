//  $Id: Herring.cxx,v 1.7 2004/08/02 12:10:01 grumbel Exp $
//
//  TuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Steve Baker <sjbaker1@airmail.net>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "tuxkart.h"
#include "material.h"
#include "Herring.h"

void Herring::update ()
{
  sgCoord c = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } } ;
 
  c . hpr [ 0 ] = h ;
 
  h += 5.0f ;
 
  tr -> setTransform ( &c ) ;
}


Herring::Herring ( sgVec3 colour )
{
  ssgVertexArray   *va = new ssgVertexArray   () ; sgVec3 v ;
  ssgNormalArray   *na = new ssgNormalArray   () ; sgVec3 n ;
  ssgColourArray   *ca = new ssgColourArray   () ; sgVec4 c ;
  ssgTexCoordArray *ta = new ssgTexCoordArray () ; sgVec2 t ;

  sgSetVec3(v, -0.5, 0.0, 0.0 ) ; va->add(v) ;
  sgSetVec3(v,  0.5, 0.0, 0.0 ) ; va->add(v) ;
  sgSetVec3(v, -0.5, 0.0, 0.5 ) ; va->add(v) ;
  sgSetVec3(v,  0.5, 0.0, 0.5 ) ; va->add(v) ;
  sgSetVec3(v, -0.5, 0.0, 0.0 ) ; va->add(v) ;
  sgSetVec3(v,  0.5, 0.0, 0.0 ) ; va->add(v) ;

  sgSetVec3(n,  0.0f,  1.0f,  0.0f ) ; na->add(n) ;

  sgCopyVec3 ( c, colour ) ; c[ 3 ] = 1.0f ; ca->add(c) ;
 
  sgSetVec2(t, 0.0, 0.0 ) ; ta->add(t) ;
  sgSetVec2(t, 1.0, 0.0 ) ; ta->add(t) ;
  sgSetVec2(t, 0.0, 1.0 ) ; ta->add(t) ;
  sgSetVec2(t, 1.0, 1.0 ) ; ta->add(t) ;
  sgSetVec2(t, 0.0, 0.0 ) ; ta->add(t) ;
  sgSetVec2(t, 1.0, 0.0 ) ; ta->add(t) ;
 

  ssgLeaf *gset = new ssgVtxTable ( GL_TRIANGLE_STRIP, va, na, ta, ca ) ;
 
  gset -> setState ( getMaterial ( "herring.rgb" ) -> getState () ) ;
  
  h = 0.0f ;
 
  sh = new Shadow ( -0.5, 0.5, -0.25, 0.25 ) ;
 
  tr = new ssgTransform () ;
 
  tr -> addKid ( sh -> getRoot () ) ;
  tr -> addKid ( gset ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}

Herring::Herring ( ssgEntity* model )
{
  ssgEntity *gset = model ;
  
  //turn off collision detection. 
  gset -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ; 
  
  h = 0.0f ;
 
  sh = new Shadow ( -0.5, 0.5, -0.25, 0.25 ) ;
 
  tr = new ssgTransform () ;
 
  tr -> addKid ( sh -> getRoot () ) ;
  tr -> addKid ( gset ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}


 
Shadow::Shadow ( float x1, float x2, float y1, float y2 )
{
  ssgVertexArray   *va = new ssgVertexArray   () ; sgVec3 v ;
  ssgNormalArray   *na = new ssgNormalArray   () ; sgVec3 n ;
  ssgColourArray   *ca = new ssgColourArray   () ; sgVec4 c ;
  ssgTexCoordArray *ta = new ssgTexCoordArray () ; sgVec2 t ;

  sgSetVec4 ( c, 0.0f, 0.0f, 0.0f, 1.0f ) ; ca->add(c) ;
  sgSetVec3 ( n, 0.0f, 0.0f, 1.0f ) ; na->add(n) ;
 
  sgSetVec3 ( v, x1, y1, 0.10 ) ; va->add(v) ;
  sgSetVec3 ( v, x2, y1, 0.10 ) ; va->add(v) ;
  sgSetVec3 ( v, x1, y2, 0.10 ) ; va->add(v) ;
  sgSetVec3 ( v, x2, y2, 0.10 ) ; va->add(v) ;
 
  sgSetVec2 ( t, 0.0, 0.0 ) ; ta->add(t) ;
  sgSetVec2 ( t, 1.0, 0.0 ) ; ta->add(t) ;
  sgSetVec2 ( t, 0.0, 1.0 ) ; ta->add(t) ;
  sgSetVec2 ( t, 1.0, 1.0 ) ; ta->add(t) ;
 
  sh = new ssgBranch ;
  sh -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
 
  sh -> setName ( "Shadow" ) ;
 
  ssgVtxTable *gs = new ssgVtxTable ( GL_TRIANGLE_STRIP, va, na, ta, ca ) ;
 
  gs -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  gs -> setState ( getMaterial ( "fuzzy.rgb" ) -> getState () ) ;
  sh -> addKid ( gs ) ;
  sh -> ref () ; /* Make sure it doesn't get deleted by mistake */
}


void HerringInstance::update ()
{
  if ( ! eaten || her == NULL ) return ;

  float t = time_to_return - fclock->getAbsTime () ;

  if ( t > 0 )
  {
    sgVec3 hell ;
    sgCopyVec3 ( hell, xyz ) ;

    if ( t > 1.0f )
      hell [ 2 ] = -1000000.0f ;
    else
      hell [ 2 ] = -t / 2.0f ;

    scs -> setTransform ( hell ) ;
  }
  else
  {
    eaten = FALSE ;
    scs -> setTransform ( xyz ) ;
  }
}


