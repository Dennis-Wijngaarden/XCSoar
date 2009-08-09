/*
Copyright_License {

  XCSoar Glide Computer - http://xcsoar.sourceforge.net/
  Copyright (C) 2000 - 2008

  	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

}
*/

#include "StdAfx.h"


#include "externs.h"
#include "Utils.h"
#include "Parser.h"
#include "Port.h"

#include "devVega.h"


BOOL nmoParseNMEA(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *GPS_INFO){

  (void) d;
  (void) String;
  (void) GPS_INFO;

  return FALSE;

}


BOOL nmoIsLogger(PDeviceDescriptor_t d){
  (void)d;
  return(FALSE);
}

BOOL nmoIsGPSSource(PDeviceDescriptor_t d){
  (void)d;
  return(FALSE);  // this is only true if GPS source is connected on VEGA.NmeaIn
}

BOOL nmoIsBaroSource(PDeviceDescriptor_t d){
  (void)d;
  return(FALSE);
}

BOOL nmoPutVoice(PDeviceDescriptor_t d, TCHAR *Sentence){
  return(FALSE);
}

BOOL nmoPutQNH(DeviceDescriptor_t *d, double NewQNH){
  (void)NewQNH;
  return(FALSE);
}


BOOL nmoInstall(PDeviceDescriptor_t d){

  d->ParseNMEA = nmoParseNMEA;
  d->PutMacCready = NULL;
  d->PutBugs = NULL;
  d->PutBallast = NULL;
  d->Open = NULL;
  d->Close = NULL;
  d->LinkTimeout = NULL;
  d->Declare = NULL;
  d->IsLogger = nmoIsLogger;
  d->IsGPSSource = nmoIsGPSSource;
  d->IsBaroSource = nmoIsBaroSource;
  d->PutVoice = nmoPutVoice;
  d->PutQNH = nmoPutQNH;

  return(TRUE);

}


BOOL nmoRegister(void){
  return(devRegister(
    TEXT("NmeaOut"),
    (1l << dfNmeaOut),
    nmoInstall
  ));
}

