//
//  LLAirport.h
//  LocusLabsSDK
//
//  Created by Samuel Ziegler on 6/12/14.
//  Copyright (c) 2014 LocusLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LLJavaScriptObject.h"
#import "LLConfiguration.h"
#import "LLBeacon.h"
#import "LLLocation.h"
#import "LLPlace.h"

@class LLAirport;
@class LLBuilding;
@class LLFloor;
@class LLNavigationPath;
@class LLSearch;
@class LLPOIDatabase;
@class LLPosition;
@class LLJavaScriptFunction;

/**
 *  Defines the delegate methods of an Airport object.  Several methods within the Airport object may require asynchronous calls to the LocusLabs
 *  servers.  These delegate methods are called when those asynchronous calls return.
 */
@protocol LLAirportDelegate <NSObject>

@optional

/**
 *  Returns the list of buildings generated by a call to LLAirport listBuildings
 *
 *  @param airport   the object which generated this list
 *  @param buildings an array of LLBuildingInfo objects
 */
- (void)airport:(LLAirport *)airport buildingList:(NSArray *)buildings;

/**
 *  A new instance of a LLBuilding object as loaded via the LLAirport loadBuilding: method.
 *
 *  @param airport  the object which loaded the building
 *  @param building the newly created LLBuilding object
 */
- (void)airport:(LLAirport *)airport buildingLoaded:(LLBuilding *)building;

/**
 *  The result of a navigation call.
 *
 *  @param airport			the airport object which performed the navigation
 *  @param navigationPath	the resulting navigation path
 *  @param startPosition 	the start position of the navigation
 *  @param destinations		the array of LLPosition destinations
 */
- (void)airport:(LLAirport *)airport navigationPath:(LLNavigationPath *)navigationPath from:(LLPosition *)startPosition toDestinations:(NSArray *)destinations;

/**
 *  The result of a update navigation path call.
 *
 *  @param airport			the airport object which performed the navigation
 *  @param navigationPath	the resulting navigation path
 *  @param startPosition 	the start position of the navigation
 *  @param destinations		the array of LLPosition destinations
 */
- (void)airport:(LLAirport *)airport updatePath:(LLNavigationPath *)navigationPath from:(LLPosition *)startPosition to:(LLPosition*)endPosition;

/**
 *  The result of a timeEstimate call.
 *
 *  @param airport			the airport object which performed the navigation
 *  @param timeEstimate     the time to traverse from the start to the end position
 */
- (void)airport:(LLAirport *)airport timeEstimate:(NSNumber*)timeEstimate from:(LLPosition *)startPosition to:(LLPosition *)endPosition;


- (void)airport:(LLAirport *)airport graphPoints:(NSArray *)points;
- (void)airport:(LLAirport *)airport path:(NSArray *)points;


@end

/**
 *  The LLAirport class provides the ability to navigate and search an airport as well as providing access to the buildings found within the airport.
 *  Instances of this class should never be created directly.  They must be generated via the loadAirport: method of LLAirportDatabase.
 */
@interface LLAirport : LLLocation

/**
 *  Delegate to handle the results of the asynchronous calls on this object
 */
@property (weak, nonatomic) id<LLAirportDelegate> delegate;

/**
 * Contains the id of the default building for this airport
 */
@property (nonatomic,readonly) NSString *defaultBuildingId;


/**
 * Returns the ordinal associated with the given floorId.
 */
-(NSNumber*)getOrdinalForFloorId:(NSString*)floorId;

/**
 * Returns true or false.  True if the provided latLng is
 * in the venue.  
 */
-(NSNumber*)isLatLngInVenue:(LLLatLng*)latLng;

/**
 *  The IATA or ICAO code for this airport
 *
 *  @return the airport code
 */
@property (nonatomic,readonly) NSString *airportCode;

/**
 *  The venue identifier for this airport
 *
 *  @return the venue id
 */
@property (nonatomic,readonly) NSString *venueId;


/**
 * The beacon regions for this airport.
 */
@property (nonatomic,readonly) NSArray *beaconRegions;

@property (nonatomic,readonly) LLPosition *position;

@property (nonatomic,readonly) NSString *positionState;

@property (nonatomic,readonly) LLBeacon *closestBeacon;

@property (nonatomic,readonly) NSString *assetVersion;

@property (nonatomic,readonly) NSString *version;

@property (nonatomic,readonly) NSArray *positioningSupported;

@property (nonatomic,readonly) NSArray *grabPoiIds;

@property (nonatomic,readonly) NSNumber *hasVouchers;

@property (nonatomic,readonly) NSString *uber;

@property (nonatomic,readonly) BOOL supportsStepwiseDirections;

@property (nonatomic,readonly) LLPlaceConfiguration *places;

@property (nonatomic,readonly) NSArray<NSString*> *searchSuggestions;

/*
* Should we be using positioning sensors for location?
*
* (A PositionSensor sends periodic information about position that includes a LatLng and errorRadius)
*/
typedef enum LLPositioningSensorAlgorithm
{
    LLPositioningSensorAlgorithmInternal = 0, // default: use our own positioning
    LLPositioningSensorAlgorithmHybrid,       // combine our own positioning with a third party's positioning
    LLPositioningSensorAlgorithmExternal      // use a third party's positioning instead of our own
} LLPositioningSensorAlgorithm;

@property(nonatomic) LLPositioningSensorAlgorithm positioningSensorAlgorithm;

@property(nonatomic) double beaconReadingLifespan;
@property(nonatomic) double bonusForBeingNearest;
@property(nonatomic) NSInteger minimumReadingCount;
@property(nonatomic, readonly) NSArray *positionManagerRollingCarpetRegionAlgorithmParameters;

/**
 *  Loads the navigation graph used to compute navigation paths.
 */
- (void)loadNavigationGraph;

/**
 *  Retrieve the list of buildings that make up this airport.  The result is passed back via the airport:buildingList: method of the delegate.
 */
- (NSArray *)listBuildings;

/**
 *  Load a specific building.  This method loads the data about the building and creates a new instance of LLBuilding which is returned via the delegate method.
 *
 *  @param buildingId identifies the building to load
 */
- (LLBuilding*)loadBuilding:(NSString *)buildingId;
- (LLBuilding*)loadBuildingSync:(NSString *)buildingId;

/**
 *  Calculate a navigation path from the start position to the end postion.
 *
 *  @param startPosition the starting point of the navigation
 *  @param endPosition   the end point of the navigation
 */
- (void)navigateFrom:(LLPosition *)startPosition to:(LLPosition *)endPosition;

/**
 *  Update the navigation path from the start position to the end postion.
 *
 *  @param startPosition the starting point of the navigation
 *  @param endPosition   the end point of the navigation
 */
- (void)updatePathFrom:(LLPosition *)startPosition to:(LLPosition *)endPosition;

/**
 *  Generate a navigation path from a start point to one or more destinations.
 *
 *  @param startPosition the starting point of the navigation
 *  @param destinations  a destination array of LLPosition instances
 */
- (void)navigateFrom:(LLPosition *)startPosition toDestinations:(NSArray *)destinations;

/**
 *  Calculate the time to travel from the start position to the end postion.
 *
 *  @param startPosition the starting point
 *  @param endPosition   the end point
 */
- (void)timeEstimateFrom:(LLPosition *)startPosition to:(LLPosition *)endPosition;

/**
 *  Creates a search object for quering the database within this search context.
 *
 *  @param query the query string
 *
 *  @return the LLSearch object
 */
- (LLSearch *)search;

/**
 *  Returns a new LLPOIDatabase for retrieving information about POIs found in this airport.
 *
 *  @return the POI database
 */
- (LLPOIDatabase *)poiDatabase;

- (void)positionChangedListener:(LLJavaScriptFunction *)callback;
- (void)positionStateChangedListener:(LLJavaScriptFunction *)callback;
- (void)closestBeaconChangedListener:(LLJavaScriptFunction *)callback;
- (void)processBeaconReadings:(NSArray *)beacons;

- (void)recordPositionSensorReading:(LLLatLng*)latLng errorRadius:(float)errorRadius floorId:(NSString*)floorId;
- (void)recordPositionSensorReading:(LLLatLng*)latLng errorRadius:(float)errorRadius floorId:(NSString*)floorId heading:(NSNumber *)heading;
- (void)recordPositionSensorServiceInterruption:(NSString*)description;
- (void)recordHeadingSensorReading:(NSNumber *)heading;
- (void)clearPosition;

- (void)getGraphPoints:(NSString*)floorId;
- (void)getPathPoints:(LLPosition *)startPosition toDestinations:(NSArray *)destinations;

@end
