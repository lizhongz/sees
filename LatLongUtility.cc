#include <cmath>
#include "LatLongUtility.h"

double LatLongUtility::calc_p2p_dist(const Coordinate &x, const Coordinate &y)
{
	double latX = deg2rad(x.lat);
	double lonX = deg2rad(x.lon);
	double latY = deg2rad(y.lat);
	double lonY = deg2rad(y.lon);

	double dLat = latY - latX;
	double dLon = lonY - lonX;

	// Formula: 
	// Δφ′ = ln(tan(π/4+φ2/2)/tan(π/4+φ1/2))
	// (the ‘stretched’ latitude difference)
	// if E:W line,	q = cos(φ1)	 
	// otherwise, q = Δφ/Δφ′	 
	// d = sqrt(Δφ² + q².Δλ²) * Radius_of_Earth

	double dPhi = log(tan(M_PI / 4 + latY / 2) / tan(M_PI / 4 + latX / 2));
	double q = dLat / dPhi;
	if(!isfinite(q))
	{
		q = cos(latX);
	}

	// if dLon over 180° take shorter rhumb across anti-meridian:
	if(abs(dLon) > M_PI)
	{
		dLon = (dLon > 0) ? (2 * M_PI - dLon) : (2 * M_PI + dLon);
	}

	return sqrt(dLat * dLat + q * q * dLon * dLon) * RADIUS_EARTH;
}

double LatLongUtility::calc_p2l_dist(const Coordinate &pnt, 
	const Coordinate &stPnt, const Coordinate &endPnt)
{
	// Calculate distrance from stPnt to pnt
	double s2pDist = calc_p2p_dist(stPnt, pnt);

	// Calculate angle between (stPnt, endPnt) and (stPnt, pnt)
	double angle = calc_angle(stPnt, endPnt, stPnt, pnt);
	
	// Calculate distance from the point to the line
	double dist =  s2pDist * sin(deg2rad(angle));

	return dist;
}

double LatLongUtility::calc_direction(const Coordinate &x, 
	const Coordinate &y)
{
	// Calculte bearing in radias
	double bearing = calc_bearing(x, y);

	// Convert bearing to direction in degree
	double direction = rad2deg(bearing);
	if(direction < 0)
		direction += 360.0;

	return direction;
}

void LatLongUtility::calc_destination(const Coordinate &x, 
	double brng, double dist, Coordinate &dest)
{ 
	double latX = deg2rad(x.lat);
	double lonX = deg2rad(x.lon);

	// Formula:	α = d/R	(angular distance)
 	// φ2 = φ1 + α.cos(θ)	 
 	// Δφ′ = ln( tan(π/4+φ2/2) / tan(π/4+φ1/2) )
 	// (the ‘stretched’ latitude difference)
	// if E:W line,	q = cos(φ1)	 
	// otherwise, q = Δφ/Δφ′	 
 	// Δλ = α.sin(θ)/q	 
 	// λ2 = (λ1+Δλ+π) % 2.π − π	 
 	// 
	// where φ is latitude, λ is longitude, 
	// Δλ is taking shortest route (<180°), 
	// ln is natural log and % is modulo, 
	// R is the earth’s radius
	
	double angDist	= dist / RADIUS_EARTH;
	double dLat = angDist * cos(brng);

	double latY = latX + dLat;
	double dPhi = log(tan(M_PI / 4 + latY / 2) 
		/ tan(M_PI / 4 + latX / 2));
	double q = dLat / dPhi;
	if(!isfinite(q)) // E-W line gives dPhi=0
	{
		q = cos(latX);
	}
	double dLon = angDist * sin(brng) / q;

	// check for some daft bugger going past the pole, 
	// normalise latitude if so	
	if(abs(latY) > (M_PI / 2)) 
	{
		latY = (latY > 0) ? (M_PI - latY) 
			: (-1 * M_PI - latY);
	}

	double lonY = fmod((lonX + dLon + 3 * M_PI), 2 * M_PI) - M_PI;

	dest.lat = rad2deg(latY);
	dest.lon = rad2deg(lonY);
}

double LatLongUtility::calc_bearing(const Coordinate &x, 
	const Coordinate &y)
{
	double latX = deg2rad(x.lat);
	double lonX = deg2rad(x.lon);
	double latY = deg2rad(y.lat);
	double lonY = deg2rad(y.lon);

	double dLat = latY - latX;
	double dLon = lonY - lonX;

	// Formula: 
	// Δφ′ = ln(tan(π/4+φ2/2)/tan(π/4+φ1/2)) 
	// (the ‘stretched’ latitude difference)
	// if E:W line,	q = cos(φ1)	 
	// otherwise, q = Δφ/Δφ′	 
	// θ = atan2(Δλ, Δφ′)
	
	double dPhi = log(tan(M_PI / 4 + latY / 2) 
		/ tan(M_PI / 4 + latX / 2));

	// if dLon over 180° take shorter rhumb across anti-meridian:
	if(abs(dLon) > M_PI)
	{
		dLon = (dLon > 0) ? (2 * M_PI - dLon) 
			: (2 * M_PI + dLon);
	}

	return atan2(dLon, dPhi);
}

double LatLongUtility::calc_angle(const Coordinate &stPntL1, 
	const Coordinate &endPntL1, const Coordinate &stPntL2, 
	const Coordinate &endPntL2)
{
	// Caclculate the angle by using two lines' bearing
	double angle = calc_bearing(stPntL2, endPntL2) 
		- calc_bearing(stPntL1, endPntL1);

	// Convert from radian to degree with range -180~180 
	if(angle > M_PI)
	{
		angle -= 2 * M_PI;
	}
	else if(angle < (-1 * M_PI))
	{
		angle += 2 * M_PI;
	}
	return rad2deg(angle);
}

double LatLongUtility::deg2rad(double deg)
{
	return deg * (M_PI / 180.0);
}

double LatLongUtility::rad2deg(double rad)
{
	return rad * (180.0 / M_PI);
}
