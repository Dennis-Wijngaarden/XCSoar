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

#include "devZander.h"

static BOOL PZAN1(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *aGPS_INFO);
static BOOL PZAN2(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *aGPS_INFO);


static BOOL ZanderParseNMEA(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *aGPS_INFO){
  (void)d;

  if(_tcsncmp(TEXT("$PZAN1"), String, 6)==0)
    {
      return PZAN1(d, &String[7], aGPS_INFO);
    }
  if(_tcsncmp(TEXT("$PZAN2"), String, 6)==0)
    {
      return PZAN2(d, &String[7], aGPS_INFO);
    }

  return FALSE;

}


/*
static BOOL ZanderIsLogger(PDeviceDescriptor_t d){
  (void)d;
  return(FALSE);
}
*/


static BOOL ZanderIsGPSSource(PDeviceDescriptor_t d){
  (void)d;
  return(TRUE);
}


static BOOL ZanderIsBaroSource(PDeviceDescriptor_t d){
	(void)d;
  return(TRUE);
}


static BOOL zanderInstall(PDeviceDescriptor_t d){

  d->ParseNMEA = ZanderParseNMEA;
  d->PutMacCready = NULL;
  d->PutBugs = NULL;
  d->PutBallast = NULL;
  d->Open = NULL;
  d->Close = NULL;
  d->Declare = NULL;
  d->IsGPSSource = ZanderIsGPSSource;
  d->IsBaroSource = ZanderIsBaroSource;

  return(TRUE);

}


BOOL zanderRegister(void){
  return(devRegister(
    TEXT("Zander"),
    (1l << dfGPS)
    | (1l << dfBaroAlt)
    | (1l << dfSpeed)
    | (1l << dfVario),
    zanderInstall
  ));
}


// *****************************************************************************
// local stuff

static BOOL PZAN1(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *aGPS_INFO)
{
  TCHAR ctemp[80];
  aGPS_INFO->BaroAltitudeAvailable = TRUE;
  NMEAParser::ExtractParameter(String,ctemp,0);
  aGPS_INFO->BaroAltitude = AltitudeToQNHAltitude(StrToDouble(ctemp,NULL));
  return TRUE;
}


static BOOL PZAN2(PDeviceDescriptor_t d, TCHAR *String, NMEA_INFO *aGPS_INFO)
{
  TCHAR ctemp[80];
  double vtas, wnet, vias;

  NMEAParser::ExtractParameter(String,ctemp,0);
  vtas = StrToDouble(ctemp,NULL)/3.6;
  // JMW 20080721 fixed km/h->m/s conversion

  NMEAParser::ExtractParameter(String,ctemp,1);
  wnet = (StrToDouble(ctemp,NULL)-10000)/100; // cm/s
  aGPS_INFO->Vario = wnet;

  if (aGPS_INFO->BaroAltitudeAvailable) {
    vias = vtas/AirDensityRatio(aGPS_INFO->BaroAltitude);
  } else {
    vias = 0.0;
  }

  aGPS_INFO->AirspeedAvailable = TRUE;
  aGPS_INFO->TrueAirspeed = vtas;
  aGPS_INFO->IndicatedAirspeed = vias;
  aGPS_INFO->VarioAvailable = TRUE;

  TriggerVarioUpdate();

  return TRUE;
}
