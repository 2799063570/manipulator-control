#ifndef EXAMPLE_8_H
#define EXAMPLE_8_H

#include "AuboRobotMetaType.h"
#include "serviceinterface.h"
class Example_8
{
public:
    /**
     * @brief demo
     *
     * Positive and negative solution interface
     */
    static void demo();

    static void FK(ServiceInterface &robotService);

    static void IK(ServiceInterface &robotService);

    static int toolPositionOriIk();
    static int baseToUserCoordinate();

    static int toolToEnd_demo();
    static int CoordinateCalibration_demo();

    static int FK_IK();
};

#endif // EXAMPLE_8_H
