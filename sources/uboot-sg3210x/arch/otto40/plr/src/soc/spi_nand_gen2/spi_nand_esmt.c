#include <spi_nand/spi_nand_ctrl.h>
#include <spi_nand/spi_nand_common.h>
#include <util.h>
#ifdef CONFIG_SPI_NAND_FLASH_INIT_FIRST
    #include <spi_nand/spi_nand_util.h>
#endif
#if defined(CONFIG_UNDER_UBOOT) && !defined(CONFIG_SPI_NAND_FLASH_INIT_FIRST)
    #include <spi_nand/spi_nand_blr_util.h>
    #include <spi_nand/spi_nand_symb_func.h>
#endif

/***********************************************
  *  ESMT's ID Definition
  ***********************************************/
#define MID_ESMT       (0xC8)
#define DID_F50L1G41LB (0x01)


// policy decision
    //input: #define NSU_PROHIBIT_QIO, or NSU_PROHIBIT_DIO  (in project/info.in)
    //       #define NSU_ESMT_USING_QIO, NSU_ESMT_USING_DIO, NSU_ESMT_USING_SIO  (in project/info.in)
    //       #define NSU_DRIVER_IN_ROM, IS_RECYCLE_SECTION_EXIST (in template/info.in)
    //       #define NSU_USING_SYMBOL_TABLE_FUNCTION (in project/info.in)

    //output: #define __DEVICE_REASSIGN, __DEVICE_USING_SIO, __DEVICE_USING_DIO, and __DEVICE_USING_QIO
    //        #define __SECTION_INIT_PHASE, __SECTION_INIT_PHASE_DATA
    //        #define __SECTION_RUNTIME, __SECTION_RUNTIME_DATA

#ifdef NSU_DRIVER_IN_ROM
    #define __SECTION_INIT_PHASE      SECTION_SPI_NAND
    #define __SECTION_INIT_PHASE_DATA SECTION_SPI_NAND_DATA
    #define __SECTION_RUNTIME         SECTION_SPI_NAND
    #define __SECTION_RUNTIME_DATA    SECTION_SPI_NAND_DATA
    #if defined(NSU_PROHIBIT_QIO) || defined(NSU_PROHIBIT_DIO)
        #error 'lplr should not run at ...'
    #endif
    #ifdef IS_RECYCLE_SECTION_EXIST
        #error 'lplr should not have recycle section ...'
    #endif
    #define __DEVICE_USING_SIO 1
    #define __DEVICE_USING_DIO 0
    #define __DEVICE_USING_QIO 0
#else
    #ifdef NSU_USING_SYMBOL_TABLE_FUNCTION
        #define __DEVICE_REASSIGN 1
    #endif
    #ifdef IS_RECYCLE_SECTION_EXIST
        #define __SECTION_INIT_PHASE        SECTION_RECYCLE
        #define __SECTION_INIT_PHASE_DATA   SECTION_RECYCLE_DATA
        #define __SECTION_RUNTIME           SECTION_UNS_TEXT
        #define __SECTION_RUNTIME_DATA      SECTION_UNS_RO
    #else
        #define __SECTION_INIT_PHASE
        #define __SECTION_INIT_PHASE_DATA
        #define __SECTION_RUNTIME
        #define __SECTION_RUNTIME_DATA
    #endif

    #ifdef NSU_ESMT_USING_QIO
        #if defined(NSU_PROHIBIT_QIO) && defined(NSU_PROHIBIT_DIO)
            #define __DEVICE_USING_SIO 1
            #define __DEVICE_USING_DIO 0
            #define __DEVICE_USING_QIO 0
        #elif defined(NSU_PROHIBIT_QIO)
            #define __DEVICE_USING_SIO 0
            #define __DEVICE_USING_DIO 1
            #define __DEVICE_USING_QIO 0
        #else
            #define __DEVICE_USING_SIO 0
            #define __DEVICE_USING_DIO 0
            #define __DEVICE_USING_QIO 1
        #endif
    #elif defined(NSU_ESMT_USING_DIO)
        #if defined(NSU_PROHIBIT_DIO)
            #define __DEVICE_USING_SIO 1
            #define __DEVICE_USING_DIO 0
            #define __DEVICE_USING_QIO 0
        #else
            #define __DEVICE_USING_SIO 0
            #define __DEVICE_USING_DIO 1
            #define __DEVICE_USING_QIO 0
        #endif
    #else
        #define __DEVICE_USING_SIO 1
        #define __DEVICE_USING_DIO 0
        #define __DEVICE_USING_QIO 0
    #endif
#endif

#ifdef CONFIG_SPI_NAND_FLASH_INIT_FIRST
#if __DEVICE_USING_QIO
__SECTION_INIT_PHASE_DATA
spi_nand_cmd_info_t esmt_qio_cmd_info = {
    .w_cmd = PROGRAM_LOAD_X4_OP,
    .w_addr_io = SIO_WIDTH,
    .w_data_io = QIO_WIDTH,
    .r_cmd = FAST_READ_QIO_OP,
    .r_addr_io = QIO_WIDTH,
    .r_data_io = QIO_WIDTH,
    .r_dummy_cycles  = 16,
};
#endif

__SECTION_RUNTIME void
esmt_ecc_encode(u32_t ecc_ability, void *dma_addr, void *fake_ptr_cs) { return; }

__SECTION_RUNTIME s32_t
esmt_ecc_decode(u32_t ecc_ability, void *dma_addr, void *fake_ptr_cs)
{
    return 0;
}

__SECTION_INIT_PHASE_DATA
spi_nand_flash_info_t esmt_chip_info[] = {
    {
        .man_id              = MID_ESMT,
        .dev_id              = DID_F50L1G41LB,
        ._num_block          = SNAF_MODEL_NUM_BLK_1024,
        ._num_page_per_block = SNAF_MODEL_NUM_PAGE_64,
        ._page_size          = SNAF_MODEL_PAGE_SIZE_2048B,
        ._spare_size         = SNAF_MODEL_SPARE_SIZE_64B,
        ._oob_size           = SNAF_MODEL_OOB_SIZE(24),
        ._ecc_ability        = ECC_MODEL_6T,
    #if __DEVICE_REASSIGN
        ._ecc_encode         = VZERO,
        ._ecc_decode         = VZERO,
        ._reset              = VZERO,
        ._cmd_info           = VZERO,
        ._model_info         = VZERO,
    #else
        ._ecc_encode         = esmt_ecc_encode,
        ._ecc_decode         = esmt_ecc_decode,
        ._reset              = nsu_reset_spi_nand_chip,
        ._model_info         = &snaf_ode_general_model,
        #if __DEVICE_USING_QIO
        ._cmd_info           = &esmt_qio_cmd_info,
        #elif __DEVICE_USING_DIO
        ._cmd_info           = &nsc_dio_cmd_info,
        #else
        ._cmd_info           = &nsc_sio_cmd_info,
        #endif
    #endif
    }
};
#endif // CONFIG_SPI_NAND_FLASH_INIT_FIRST

__SECTION_INIT_PHASE u32_t
esmt_read_id(u32_t cs)
{
    u32_t dummy = 0x00;
    u32_t w_io_len = IO_WIDTH_LEN(SIO_WIDTH,CMR_LEN(2));
    u32_t r_io_len = IO_WIDTH_LEN(SIO_WIDTH,CMR_LEN(2));

    u32_t ret = nsu_read_spi_nand_id(cs, dummy, w_io_len, r_io_len);
    return ((ret>>16)&0xFFFF);
}


#ifdef CONFIG_SPI_NAND_FLASH_INIT_FIRST
__SECTION_INIT_PHASE spi_nand_flash_info_t *
probe_esmt_spi_nand_chip(void)
{
    const u32_t cs = 0;
    u32_t i, rdid;
    u16_t did;

    nsu_reset_spi_nand_chip(cs);
    rdid = esmt_read_id(cs);
    did = rdid&0xFF;

    if(MID_ESMT != (rdid>>8)) return VZERO;

    for(i=0 ; i<ELEMENT_OF_SNAF_INFO(esmt_chip_info) ; i++){
        if(esmt_chip_info[i].dev_id == did)
        {
            #if __DEVICE_REASSIGN
                #if __DEVICE_USING_QIO
                esmt_chip_info[i]._cmd_info = &esmt_qio_cmd_info;
                #elif __DEVICE_USING_DIO
                esmt_chip_info[i]._cmd_info = _nsu_dio_cmd_info_ptr;
                #else
                esmt_chip_info[i]._cmd_info = _nsu_cmd_info_ptr;
                #endif
                esmt_chip_info[i]._model_info= &nsu_model_info;
                esmt_chip_info[i]._reset     = _nsu_reset_ptr;
                if(ECC_USE_ODE==esmt_chip_info[i]._ecc_ability) {
                    nsu_model_info._page_read_ecc  = _nsu_page_read_with_ode_ptr;
                    nsu_model_info._page_write_ecc = _nsu_page_write_with_ode_ptr;
                    esmt_chip_info[i]._ecc_encode = esmt_ecc_encode;
                    esmt_chip_info[i]._ecc_decode = esmt_ecc_decode;
                } else {
                    esmt_chip_info[i]._ecc_encode = _nsu_ecc_encode_ptr;
                    esmt_chip_info[i]._ecc_decode = _nsu_ecc_decode_ptr;
                }
            #endif
            if(ECC_USE_ODE==esmt_chip_info[i]._ecc_ability) {
                nsu_enable_on_die_ecc(cs);
            } else {
                nsu_disable_on_die_ecc(cs);
            }

            nsu_block_unprotect(cs);

            return &esmt_chip_info[i];
        }
    }
    return VZERO;
}
REG_SPI_NAND_PROBE_FUNC(probe_esmt_spi_nand_chip);
#endif   // CONFIG_SPI_NAND_FLASH_INIT_FIRST
#ifdef CONFIG_SPI_NAND_FLASH_INIT_REST
int
esmt_init_rest(void)
{
    u32_t cs=1;
    // check ID, cs0 and cs1 should be identical
    u32_t rdid = esmt_read_id(cs);
    if(((rdid>>8)!=MID_ESMT) || ((rdid&0xFF)!=_spi_nand_info->dev_id))
      { return 0; }

    // reset
    nsu_reset_spi_nand_chip(cs);

    if(ECC_USE_ODE==_spi_nand_info->_ecc_ability){
        nsu_enable_on_die_ecc(cs);
    } else {
        nsu_disable_on_die_ecc(cs);
    }

    nsu_block_unprotect(cs);

    return 1;
}
REG_SPI_NAND_INIT_REST_FUNC(esmt_init_rest);
#endif // CONFIG_SPI_NAND_FLASH_INIT_REST


