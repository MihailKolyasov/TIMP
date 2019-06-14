/** @file
 * @author Колясов М.М.
 * @version 1.0
 * @date 13.06.2019
 * @copyright ИБСТ ПГУ
 * @warning Это учебный пример
 * @brief Заголовочный файл для модуля Стеганографической программы методом LSB для изображений в формате BMP
 */
class BmpFile
{
    /** @brief Стеганография
    * @details Контейнер устанавливается в конструкторе.
    * Для записи и извлечения преднзначены соответственно методы 
    * hide, unhide.
    * @warning Только для изображений формата BMP
    */
public:
    
    int bmpIdentifier;
    long bmpFilesize;											
    unsigned short int bmpres1,bmpres2;
    long bmpPixoff;
    long bmpiSize;
    long bmpWidth;													
    long bmpHeight;													
    unsigned short int bmpPlanes;
    unsigned short int bmpBitsPixel;
    long bmpCompression;											
    long bmpImageSize;
    long bmpXscale;
    long bmpYScale;
    long bmpColor;													
    long bmpImpCol;
    long bmpTotalStuffablechar;										
 /**
    * @brief конструктор для считывания файла
    * @param [in] filename файл. Формат BMP
    * @throw cipher_error, если неверный формат изображения.
    */
    BmpFile(char *filename);                                		
    ~BmpFile();
        /**
    * @brief открытие файла с изображением.
    * @param [in] filename файл. Формат BMP
    * @return целочисленное значение.
    * @throw cipher_error, если открыть не получилось.
    */                                     		
    int isFileExist(char *filename);
    /**
    * @brief получение высоты и ширины изображения.
    * @param [in] filename файл. Формат BMP
    * @param [in] width ширина.
    * @param [in] height высота.
    * @return целочисленное значение.
    * @throw cipher_error, если неверный формат изображения.
    */
    int getDimension(char *filename, long *width, long *height);	
        /**
    * @brief чтение загаловка файла с изображением.
    * @param [in] filename файл. Формат BMP
    * @return целочисленное значение.
    * @throw cipher_error, если неверный формат изображения.
    */   
    int readHeader(char *filename);
    /**
    * @brief сокрытие информации.
    * @param [in] bmpfile файл. Формат BMP
    * @param [in] txtfile файл.
    * @param [in] output файл. Формат BMP
    * @return целочисленное значение.
    * @throw cipher_error, если неверный формат изображения.
    * если не удалось открыть какой-то из файлов
    * сообщение длинне возможного сокрытия.
    */
    int hide(char *bmpfile, char *txtfile, char *output);
      /**
    * @brief извлечение информации.
    * @param [in] bmpfile файл. Формат BMP
    * @param [in] txtfile файл.
    * @return целочисленное значение.
    * @throw cipher_error, если неверный формат изображения.
    * если не удалось открыть какой-то из файлов
    */
    int unhide(char *bmpfile,char *txtfile);
      /**
    * @brief Проверка файлов для сокрытия.
    * @param [in] bmpfile файл. Формат BMP
    * @param [in] txtfile файл.
    * @return целочисленное значение.
    * @throw cipher_error, если неверный формат изображения.
    * если не удалось открыть какой-то из файлов
    * сообщение длинне возможного сокрытия.
    */
    int checkFilesForHiding(char *bmpfile, char *txtfile);
      /**
    * @brief вывод информации о изображении
    * @return целочисленное значение.
    */
    int printFileInfo();
};

class cipher_error: public std::invalid_argument
{
public:
    explicit cipher_error (const std::string& what_arg):
        std::invalid_argument(what_arg) {}
    explicit cipher_error (const char* what_arg):
        std::invalid_argument(what_arg) {}
};
