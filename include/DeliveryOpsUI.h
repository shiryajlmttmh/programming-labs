#pragma once

class DeliveryService;

void handle_assign_order(DeliveryService& delivery_service);
void handle_complete_delivery(DeliveryService& delivery_service);
void handle_all_specific_actions(DeliveryService& delivery_service);
void handle_delivery_costs(DeliveryService& delivery_service);