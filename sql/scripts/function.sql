--Эта функция принимает book_id в качестве аргумента и возвращает средний рейтинг для этой книги.l
drop function get_average_book_rating(integer)
CREATE OR REPLACE FUNCTION store.get_average_book_rating(id_book INT)
RETURNS NUMERIC(3, 2) AS $$
DECLARE
    avg_rating NUMERIC(3, 2);
BEGIN
    SELECT AVG(rating) INTO avg_rating
    FROM store.reviews
    WHERE book_id = get_average_book_rating.id_book;

    RETURN avg_rating;
END;
$$ LANGUAGE plpgsql;

SELECT store.get_average_book_rating(1);

--Эта функция принимает customer_id, начальную и конечную даты в качестве аргументов и возвращает общее количество заказов, сделанных этим клиентом в указанном диапазоне дат.

CREATE OR REPLACE FUNCTION store.get_customer_order_count(customer_id_function INT, start_date DATE, end_date DATE)
RETURNS INTEGER AS $$
DECLARE
    order_count INTEGER;
BEGIN
    SELECT COUNT(*) INTO order_count
    FROM store.orders
    WHERE customer_id = get_customer_order_count.customer_id_function
      AND order_date BETWEEN start_date AND end_date;
    RETURN order_count;
END;
$$ LANGUAGE plpgsql;

SELECT store.get_customer_order_count(1, '2020-01-01', '2025-12-31');

--Эта хранимая процедура добавляет нового клиента в таблицу customers и записывает информацию о добавлении в таблицу customer_history.

CREATE OR REPLACE PROCEDURE store.add_new_customer(
    customer_name VARCHAR(50),
    customer_last_name VARCHAR(50),
    customer_email VARCHAR(150),
    customer_phone VARCHAR(50)
)
LANGUAGE plpgsql
AS $$
DECLARE
    new_customer_id INT;
BEGIN
    INSERT INTO store.customers (customer_name, customer_last_name, customer_email, customer_phone)
    VALUES (add_new_customer.customer_name, add_new_customer.customer_last_name, add_new_customer.customer_email, add_new_customer.customer_phone)
    RETURNING customer_id INTO new_customer_id;
    INSERT INTO store.customer_history (customer_id, customer_name, customer_last_name, customer_email, customer_phone, date_register)
    VALUES (new_customer_id, add_new_customer.customer_name, add_new_customer.customer_last_name, add_new_customer.customer_email, add_new_customer.customer_phone, NOW());
    COMMIT;
END;
$$;

CALL store.add_new_customer('John', 'Diz', 'john.diz@rambler.com', '123-456-7890-89');
