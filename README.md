# GLO192 Display Driver

Данный проект является драйверами для работы с дисплеем GLO192

## Применение

В основном файле вызвать функцию GLO192 и передать в аргументы указатели на функции изменения состояния A0, nRST, CS и отправки данные SendData(SPI)

В файле Display_screens:
1)При необходимости создания дополнительного "слоя" - копируем инициализацию структур ScreensMain с необходимым количество экранов (в квадратных скобках), screenMain как указатель на текущий экран и ScreenMainMode как состояние текущего экрана
2)Инициализация нового "слоя" производится так же, как и в функции SreensInit

Для примера, кроме MainScreen было создано 3 экрана:
1) BME280DataScreen - пример экрана для построчной записи данных на экран
2) SecondScreen - применение функции IntermediateScreen - эта функция преднаначена для вывода промежуточного экрана между "слоями"
3) ThirdScreen - применение функции SwitchScreen - эта функция преднаначена для вывода экрана для изменения bool значения (Светодиод, реле и т.п.)

Так же доступно пропорциональное увелечение шрифта (в 2, 3 и 4 раза) - для этого необходимо изменить RATIO_1_TO_1 на необходимое (иначальная размерность шрифта 6x8)