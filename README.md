# Хворостяной Валерий Дмитриевич, БПИ217

# Домашнее задание №4
Разработка многопоточного приложения (на оценку 6).

## Вариант задания 25: 
9. Задача о супермаркете. В супермаркете работают два кассира, покупатели заходят в супермаркет, делают покупки и становятся в очередь к случайному кассиру. Пока очередь пуста, кассир «спит», как только появляется покупатель, кассир «просыпается». Покупатель «спит» в очереди, пока не
подойдет к кассиру. Создать многопоточное приложение, моделирующее
рабочий день супермаркета.

1. **Парадигма «Клиент - сервер»** 

Термины «клиент» и «сервер» часто применяются к различным видам программных приложений. Парадигма «клиент-сервер» состоит в разделении работы на две части, представляемые процессами или потоками. Одна часть, клиент, создает запросы на получение данных либо действий. Другая часть, сервер, выполняет эти запросы. Роли запрашивающей и отвечающей стороны в большинстве случаев определяются логикой самих приложений. Термины «клиент-сервер» используются на уровне операционной системы для описания отношений типа «изготовитель-потребитель», которые могут существовать между процессами. Например, если для взаимодействия двух процессов используется FIFO-очередь, один из процессов «играет» роль сервера, а другой — роль клиента. Иногда клиент может «исполнить» роль сервера, если сам будет получать запросы. Аналогично сервер будет выступать в роли клиента, если ему потребуется обращаться с запросами к другим программам.

Источник: https://studfile.net/preview/5828091/page:111/

В данной задаче 2 сервера и сколько угодно клиентов. Клиенты внчале делают свои дела потом обращаются к серверу (встают на кассу). Далее обслуживание происходит по правилам очереди. В начале программы запрашивается положительное число - количество покупателей, при отрицательном или нецелом значении запрашивается повторный ввод.

Фактически касса - сервер и ждет когда к нему произойдет обращение, как только запрос приходит обрабатывает его. Если в процессе появились новые запросы обрабатывает их по очереди прихода. Клиент делает свои дела, затем делает запрос серверу и после уходит.

2. **В программе на Си реализовано:**

a. 2 потока. Кассиры. Для них есть семафор их работы, а также мьютекс на очередь к ним.

b. N потоков покупателей. Для них также есть семафор для их ожидания в очереди.

с. Для каждого покупателя генерируется случайное время его хождения по магазину. Также кассиры обслуживают покупатеелй также за случайное время.

d. Также есть общий мьютекс на вывод, чтобы вывод был непрерывный.

e. Использование семафор для смены потоков;

f. Результаты работы программы выводятся одновременно и на экран;

g. В консоль выводятся все события. Момент окончания совершения покупок и подход к очереди на кассу и выход из магазина.

3. **Тестовое покрытие:** 

Результат в программе на С++:
![image](https://user-images.githubusercontent.com/18235734/215675726-491bbc2b-819a-4863-bc36-c9f0a2033362.png)
