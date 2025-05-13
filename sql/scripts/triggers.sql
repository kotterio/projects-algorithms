
--триггер для добавления изменений в customer_history

CREATE OR REPLACE FUNCTION store.customer_changes()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'UPDATE' THEN
        INSERT INTO store.customer_history (customer_id, customer_name, customer_last_name, customer_email, customer_phone, date_register)
        VALUES (OLD.customer_id, OLD.customer_name, OLD.customer_last_name, OLD.customer_email, OLD.customer_phone, OLD.date_register);
        RETURN OLD;
    ELSIF TG_OP = 'DELETE' THEN
        INSERT INTO store.customer_history (customer_id, customer_name, customer_last_name, customer_email, customer_phone, date_register)
        VALUES (OLD.customer_id, OLD.customer_name, OLD.customer_last_name, OLD.customer_email, OLD.customer_phone, OLD.date_register);
        RETURN OLD;
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER customer_audit
AFTER UPDATE OR DELETE ON store.customers
FOR EACH ROW
EXECUTE PROCEDURE store.customer_changes();

--удаление отзывов при удалении книги
CREATE OR REPLACE FUNCTION store.delete_book_reviews()
RETURNS TRIGGER AS $$
BEGIN
    DELETE FROM store.reviews WHERE book_id = OLD.book_id;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER book_reviews_delete
AFTER DELETE ON store.books
FOR EACH ROW
EXECUTE PROCEDURE store.delete_book_reviews();

--Автоматическое обновление статуса status в таблице store.shipments при изменении address_id, если статус еще не Доставлено.

CREATE OR REPLACE FUNCTION store.reset_shipment_status()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.address_id <> NEW.address_id AND NEW.status <> 'Доставлено' THEN
        NEW.status := 'В обработке';  -- Или другой подходящий статус
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER shipment_address_update
BEFORE UPDATE ON store.shipments
FOR EACH ROW
EXECUTE PROCEDURE store.reset_shipment_status();
