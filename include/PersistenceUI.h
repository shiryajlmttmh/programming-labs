#pragma once

class DeliveryService;

void handle_save_state(const DeliveryService& delivery_service);
void handle_load_state(DeliveryService& delivery_service);
void handle_create_report(const DeliveryService& delivery_service);

bool initialize_state(DeliveryService& delivery_service);
void auto_save_state(const DeliveryService& delivery_service);