#ifndef MODBUS_RTU_REGISTERS
#define MODBUS_RTU_REGISTERS

/*
 * Здесь идут макросы определяющие карту регистров
 * Вся карта разделяется на подмассивы:
 *  1)  RW - Регистры доступные для чтения и записи
 *  2)  RO - Регистры доступные только для чтения
 *  3)  EMPTY - Пустые регистры (при записи в них ни чего не происходит, читаются всегда нулями),
 *      это все оставшиеся регистры, они не имеют ни каких свойств и в define-ах не фигурируют
 *      define-ами определяются только RW и RO подмассивы (они не должны не в коем случае пересекаться)
 * Каждый подмассив имеет четыре свойства:
 *  1)  ACCESSx массив в котором каждый элемент является спецификатором доступа для конкретного
 *      экземпляра ModBus-стека (в устройстве может быть не один ModBus),
 *      например, один и тотже подмассив
 *      Номер каждого элемента массива - это номер экземпляра конкретного ModBus-стека,
 *      x-порядковый номер подмассива:
 *      а) RW - Регистры доступные для чтения и записи
 *      б) RO - Регистры доступные только для чтения
 *      в) Не доступно ни чтение не запись
 *  2)  START_ADDRx массив в котором каждый элемент является начальным адресом подмассива регистров
 *      для конкретного экземпляра ModBus-стека, где x-порядковый номер подмассива
 *  3)  FINISH_ADDRx массив в котором каждый элемент является конечным адресом подмассива регистров,
 *      где x-порядковый номер подмассива
 *  4)  SIZE_ARRx = FINISH_ADDRx - START_ADDRx Размер подмассива, где x-порядковый номер подмассива
 * */



#define MODBUS_SLAVE_REGISTERS_SIZE_ARRAY(FINISH_ADDR, START_ADDR) (((FINISH_ADDR) - (START_ADDR) + 1))



// Доступ к регистрам
enum ACCESS {
    ACCESS_REG_RW       = 0x00, // а) RW - Регистры доступные для чтения и записи
    ACCESS_REG_RO       = 0x01, // б) RO - Регистры доступные только для чтения
    ACCESS_REG_NOTRW    = 0x02  // в) Не доступно ни чтение не запись
};



// Это заголовок для каждого подмассива (header)
// Размер массива равен числу заведенных ModBus-ов
struct modbus_slave_registers_subarray {
        uint8_t  ACCESS;            // Доступ к регистрам
        uint16_t START_ADDR;        // Стартовый адрес
        uint16_t FINISH_ADDR;       // Конечный адрес
        uint16_t SIZE_ARR;          // Размер массива
        uint16_t *psubarray;        // Указатель на подмассив для которого будет сделан этот заголовок
};

// Уникальная карта регистров для каждого экземпляра modbus_slave
struct modbus_slave_unique_registers_map {
    struct modbus_slave_registers_subarray *pHeaders;// Указатель на массив карт регистров
    uint8_t	NumSubArray;                             // Число подмассивов карт регистров
};


// Создать уникальную карту регистров
//-----------------------------------------------------
void ModBus_Slave_Creat_Unique_Reg_Map(
    struct modbus_slave_registers_subarray *pHeaders, // Указатель на массив карт регистров
    uint16_t *psubarray,     // Указатель на подмассив регистров (uint16_t)
    uint8_t  ACCESS,         // Доступ к регистрам
    uint16_t START_ADDR,     // Стартовый адрес
    uint16_t FINISH_ADDR);   // Конечный адрес

// Инициализация карты регистров
//------------------------------------------------------
void ModBusRTU_Registers_Init(void);
// Функция записи одного регистра ModBus с проверкой возможности записи
// *pRegmap - экземпляр уникальной карты регистров с которым ведется работа в конкретном случае (их же может быть не один штук)
// DataReg - собственно значение полученного по сети записываемого или запрошенного по сети читаемого регистра ModBus
// Address - ModBus адрес этого регистра (не путать с локальным адресом ячейки памяти)
//------------------------------------------------------
void WrightModBusReg(struct modbus_slave_unique_registers_map *pRegmap, uint16_t DataReg, uint16_t Address);
// Функция поиска заданного регистра ModBus и последующего чтения
// *pRegmap - экземпляр уникальной карты регистров с которым ведется работа в конкретном случае
// Address - ModBus адрес этого регистра (не путать с локальным адресом ячейки памяти)
// возвращаемое значение - требуемое значение регистра ModBus
//------------------------------------------------------
uint16_t ReadModBusReg(struct modbus_slave_unique_registers_map *pRegmap, uint16_t Address);

#endif // MODBUS_RTU_REGISTERS
