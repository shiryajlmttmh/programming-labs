#include "DeliveryOpsUI.h"
#include "UICommon.h"
#include "InputUtils.h"
#include "DeliveryService.h"

#include <string>

using namespace std;

void handle_assign_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа: ");
    delivery_service.assign_order_to_vehicle(order_id);

    for (size_t i = 0; i < delivery_service.get_vehicles_count(); i++)
    {
        const Vehicle& vehicle = delivery_service.get_vehicle_by_index(i);
        if (vehicle.get_current_order_id() == order_id)
            logger.log("ASSIGN_ORDER", "Заказ номер " + to_string(order_id)
                + " назначен на транспорт номер " + to_string(vehicle.get_id()));
    }
}

void handle_complete_delivery(DeliveryService& delivery_service)
{
    int vehicle_id = read_int("Введите ID транспорта, завершившего доставку: ");
    int order_id = delivery_service.get_vehicle(vehicle_id)->get_current_order_id();
    delivery_service.complete_delivery(vehicle_id);
    logger.log("COMPLETE_DELIVERY", "Транспорт номер " + to_string(vehicle_id)
        + " завершил доставку заказа номер " + to_string(order_id));
}

void handle_all_specific_actions(DeliveryService& delivery_service)
{
    delivery_service.perform_all_specific_actions();
    logger.log("SPECIFIC_ACTION", "Выполнены специфические действия всего транспорта");
}

void handle_delivery_costs(DeliveryService& delivery_service)
{
    double weight = read_double("Введите вес груза (кг): ");
    delivery_service.print_delivery_costs(weight);
}