//
//  LLPositionManager.h
//  LocusLabsSDK
//
//  Created by Samuel Ziegler on 6/16/14.
//  Copyright (c) 2014 LocusLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "LLPositionManagerDelegate.h"
#import "LLMapView.h"


#define NO_FLOOR @"NO_FLOOR"


// *********************************************************************************************************************
// OUTBOUND NOTIFICATIONS
//
// These notifications are sent FROM LLPositionManager to indicate changes reported by locuslabs.maps.PositionManager
// In these notifications, the object sent with the notification is the data
// *********************************************************************************************************************

/**
 * NOTIFICATION_POSITION_CHANGED
 *
 * The PositionManager has calculated a new position.
 *
 *  object:     an LLPosition indicating the new position
 *  userInfo:   none
 */
#define NOTIFICATION_POSITION_CHANGED                     @"Position Changed"

/**
 * NOTIFICATION_POSITION_SENSOR_STATE_CHANGED
 *
 * The PositionManager reports a change in the state of the positioning algorithm (currently used only to 
 * provide debugging information in the calling iOS application)
 *
 *  object:     NSString indicating the new state
 *  userInfo:   none
 */
#define NOTIFICATION_POSITION_SENSOR_STATE_CHANGED        @"Position Sensor State Changed"
#define NOTIFICATION_CLOSEST_BEACON_CHANGED        @"Closest beacon changed"

// *********************************************************************************************************************
// INBOUND NOTIFICATIONS
//
// These notifications are sent TO LLPositionManager to indicate changes to be used by locuslabs.maps.PositionManager
// In these notifications, data is sent in the notification's userInfo dictionary
// *********************************************************************************************************************

/**
 * NOTIFICATION_POSITION_SENSOR_POSITION_CHANGED
 *
 * A third-party positioning system (e.g. IndoorAtlas) indicates a new (non-beacon) position to be sent to the
 * PositionManager as input towards calculating the current position.
 *
 *  object:     not used
 *  userInfo:   "latLng"        LLLatLng indicating the proposed new position
 *              "errorRadius"   NSNumber indicating the accuracy of the new position (represented by a circle in meters)
 *              "floorId"       NSString with the floorId of the relevant floor
 */
#define NOTIFICATION_POSITION_SENSOR_POSITION_CHANGED     @"Position Sensor Position Changed"

/**
 * NOTIFICATION_POSITION_HEADING_CHANGED
 *
 * A third-party positioning system (e.g. IndoorAtlas) indicates a new heading to be sent to the
 * PositionManager.
 *
 *  object:     not used
 *  userInfo:   "heading"        Value between 0 - 360. 0 = North, 90 = East, 180 = South, 270 = West
 */
#define NOTIFICATION_POSITION_HEADING_CHANGED     @"Position Sensor Heading Changed"

/**
 * NOTIFICATION_POSITION_SENSOR_SERVICE_INTERRUPTION
 *
 * A third-party positioning system (e.g. IndoorAtlas) indicates that it will (probably temporarily) no longer
 * send location information.
 *
 *  object:     not used
 *  userInfo:   "description"   NSString indicating the cause of the interruption
 */
#define NOTIFICATION_POSITION_SENSOR_SERVICE_INTERRUPTION @"Position Sensor Service Interruption"

/**
 * NOTIFICATION_POSITION_CLEAR
 *
 * Clears the position.
 *
 *  object:     not used
 */
#define NOTIFICATION_POSITION_CLEAR @"Clear Position"

/**
 * NOTIFICATION_SHOULD_USE_POSITIONING_SENSORS
 *
 * A command from the calling application to use/not use the third-party positioning system (e.g. IndoorAtlas)
 *
 *  object:     not used
 *  userInfo:   "positioningSensorAlgorithm" LLPositioningSensorAlgorithm indicating which algorithm to use
 *              "positionManager"            LLPositionManager
 */
#define NOTIFICATION_SET_POSITIONING_SENSOR_ALGORITHM @"set positioning sensor algorithm"

/**
 * NOTIFICATION_CHANGE_POSITIONING_PARAMETERS
 *
 * A command from the calling application to modify parameters of the PositionManager algorithm. (The HKG demo
 * app uses this call to modify the parameters based on sliders on the front page of the app.) See PositionManager.js
 * for information about the individual parameters
 *
 *  object:     not used
 *  userInfo:   "beaconReadingLifespan" NSNumber (long) indicating the new beaconReadingLifespan to use
 *              "bonusForBeingNearest"  NSNumber (long) indicating the new bonusForBeingNearest to use
 *              "minimumReadingCount"   NSNumber (long) indicating the new minimumReadingCount to use
 */
#define NOTIFICATION_CHANGE_POSITIONING_PARAMETERS        @"Change Positioning Parameters"

/**
* NOTIFICATION_DID_ENTER_BEACON_REGION
*
* The CLLocationManager reports that the user has entered a new beacon region
*
* object:       the CLBeaconRegion entered
* userInfo:     not used
*/
#define NOTIFICATION_DID_ENTER_BEACON_REGION  (@"NOTIFICATION_DID_ENTER_BEACON_REGION")

/**
* NOTIFICATION_DID_ENTER_BEACON_REGION
*
* The CLLocationManager reports that the user has exited a beacon region
*
* object:       the CLBeaconRegion entered
* userInfo:     not used
*/
#define NOTIFICATION_DID_EXIT_BEACON_REGION   (@"NOTIFICATION_DID_EXIT_BEACON_REGION")

/**
 *  The LLPositionManager leverages all available positioning technologies to give as precise a location as possible.
 */
@interface LLPositionManager : NSObject <CLLocationManagerDelegate>

@property (retain,nonatomic) CLLocationManager *locationManager;
@property (retain,nonatomic) NSArray *beaconRegions;

/**
 *  The delegate to recieve events generated by the position manager.
 */
@property (weak,nonatomic) id<LLPositionManagerDelegate> delegate;

/**
 *  The airports are used to gather all of the information needed for indoor positioning.
 */
@property (strong,nonatomic,readonly) NSArray *airports;

/**
 *  Enable or disable active positioning by setting this property to true or false.  Enabling active positioning
 *  will provide positioning with higher accuracy and more frequent updates, but uses significantly more power and resources.  Therefore, it should
 *  only for real-time or precision features such as nagivation.  Enabling active positioning implicitly activates passive positioning.
 */
@property (assign,nonatomic) BOOL activePositioning;

/**
 *  Enable or disable passive positioning by setting this property to true or false.  Passive positioning uses minimal
 *  resources and will provide position updates when the application is in the background.
 */
@property (assign,nonatomic) BOOL passivePositioning;

/**
 *  If positioning is available on this device, this is set to true.
 */
@property (assign,nonatomic) BOOL positioningAvailable;

@property (assign,nonatomic) BOOL positioningEnabled;

/**
 *  Constructor.
 *
 *  Different Airports can use different positioning mechanisms, including Beacons,
 *  Core Location and others.  To monitor multiple airports with a single Position Manager
 *  ALL of the airports must use the same mechanism.
 *
 *  Due to this restriction it is recommended that you use a separate Position Manager for 
 *  each airport individually.
 *
 *  @param airportDatabase the airport database to use for positioning information
 *
 *  @return the new instance
 */
- (id)initWithAirports:(NSArray*)airports;
- (id)initWithAirportsBeaconsOnly:(NSArray*)airports;

- (void)startMonitoringAndRangingBeaconRegion:(CLBeaconRegion *)region;
- (void)stopMonitoringAndRangingBeaconRegion:(CLBeaconRegion *)region;

// internal-only
+ (void) requestLocationPermissionFromUser:(CLLocationManager*)locationManager;

@end
