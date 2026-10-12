#pragma once

class DeliveryService;

void handle_add_order(DeliveryService& delivery_service);
void handle_remove_order(DeliveryService& delivery_service);
void handle_manage_order(DeliveryService& delivery_service);
void handle_compare_orders(DeliveryService& delivery_service);