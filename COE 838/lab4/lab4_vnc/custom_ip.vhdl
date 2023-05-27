library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY custom_ip IS
	PORT( CLOCK_50, HPS_DDR3_RZQ,HPS_ENET_RX_CLK, HPS_ENET_RX_DV		: IN STD_LOGIC;
		  HPS_DDR3_ADDR 						: OUT STD_LOGIC_VECTOR(14 DOWNTO 0);
		  HPS_DDR3_BA							: OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
		  HPS_DDR3_CS_N							: OUT STD_LOGIC;
		  HPS_DDR3_CK_P, HPS_DDR3_CK_N, HPS_DDR3_CKE			: OUT STD_LOGIC;
		  HPS_USB_DIR, HPS_USB_NXT, HPS_USB_CLKOUT			: IN STD_LOGIC;
		  HPS_ENET_RX_DATA						: IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		  HPS_SD_DATA, HPS_DDR3_DQS_N, HPS_DDR3_DQS_P			: INOUT STD_LOGIC_VECTOR(3 DOWNTO 0);
		  HPS_ENET_MDIO							: INOUT STD_LOGIC; 
		  HPS_USB_DATA							: INOUT STD_LOGIC_VECTOR(7 DOWNTO 0); 
		  HPS_DDR3_DQ							: INOUT STD_LOGIC_VECTOR(31 DOWNTO 0);
		  HPS_SD_CMD							: INOUT STD_LOGIC;
		  HPS_ENET_TX_DATA, HPS_DDR3_DM					: OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
		  HPS_DDR3_ODT, HPS_DDR3_RAS_N, HPS_DDR3_RESET_N		: OUT STD_LOGIC;
		  HPS_DDR3_CAS_N, HPS_DDR3_WE_N					: OUT STD_LOGIC;
		  HPS_ENET_MDC, HPS_ENET_TX_EN 					: OUT STD_LOGIC;
		  HPS_USB_STP, HPS_SD_CLK, HPS_ENET_GTX_CLK      		: OUT STD_LOGIC);		  		  
END custom_ip;

ARCHITECTURE Behaviour OF custom_ip IS

	--instantiate the soc_systtem component here
   
    component soc_system is
        port (
            clk_clk                             : in    std_logic                     := 'X';             -- clk
            hps_0_h2f_reset_reset_n             : out   std_logic;                                        -- reset_n
            hps_io_hps_io_emac1_inst_TX_CLK     : out   std_logic;                                        -- hps_io_emac1_inst_TX_CLK
            hps_io_hps_io_emac1_inst_TXD0       : out   std_logic;                                        -- hps_io_emac1_inst_TXD0
            hps_io_hps_io_emac1_inst_TXD1       : out   std_logic;                                        -- hps_io_emac1_inst_TXD1
            hps_io_hps_io_emac1_inst_TXD2       : out   std_logic;                                        -- hps_io_emac1_inst_TXD2
            hps_io_hps_io_emac1_inst_TXD3       : out   std_logic;                                        -- hps_io_emac1_inst_TXD3
            hps_io_hps_io_emac1_inst_RXD0       : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD0
            hps_io_hps_io_emac1_inst_MDIO       : inout std_logic                     := 'X';             -- hps_io_emac1_inst_MDIO
            hps_io_hps_io_emac1_inst_MDC        : out   std_logic;                                        -- hps_io_emac1_inst_MDC
            hps_io_hps_io_emac1_inst_RX_CTL     : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CTL
            hps_io_hps_io_emac1_inst_TX_CTL     : out   std_logic;                                        -- hps_io_emac1_inst_TX_CTL
            hps_io_hps_io_emac1_inst_RX_CLK     : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CLK
            hps_io_hps_io_emac1_inst_RXD1       : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD1
            hps_io_hps_io_emac1_inst_RXD2       : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD2
            hps_io_hps_io_emac1_inst_RXD3       : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD3
            hps_io_hps_io_sdio_inst_CMD         : inout std_logic                     := 'X';             -- hps_io_sdio_inst_CMD
            hps_io_hps_io_sdio_inst_D0          : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D0
            hps_io_hps_io_sdio_inst_D1          : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D1
            hps_io_hps_io_sdio_inst_CLK         : out   std_logic;                                        -- hps_io_sdio_inst_CLK
            hps_io_hps_io_sdio_inst_D2          : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D2
            hps_io_hps_io_sdio_inst_D3          : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D3
            hps_io_hps_io_usb1_inst_D0          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D0
            hps_io_hps_io_usb1_inst_D1          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D1
            hps_io_hps_io_usb1_inst_D2          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D2
            hps_io_hps_io_usb1_inst_D3          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D3
            hps_io_hps_io_usb1_inst_D4          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D4
            hps_io_hps_io_usb1_inst_D5          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D5
            hps_io_hps_io_usb1_inst_D6          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D6
            hps_io_hps_io_usb1_inst_D7          : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D7
            hps_io_hps_io_usb1_inst_CLK         : in    std_logic                     := 'X';             -- hps_io_usb1_inst_CLK
            hps_io_hps_io_usb1_inst_STP         : out   std_logic;                                        -- hps_io_usb1_inst_STP
            hps_io_hps_io_usb1_inst_DIR         : in    std_logic                     := 'X';             -- hps_io_usb1_inst_DIR
            hps_io_hps_io_usb1_inst_NXT         : in    std_logic                     := 'X';             -- hps_io_usb1_inst_NXT
            memory_mem_a                        : out   std_logic_vector(14 downto 0);                    -- mem_a
            memory_mem_ba                       : out   std_logic_vector(2 downto 0);                     -- mem_ba
            memory_mem_ck                       : out   std_logic;                                        -- mem_ck
            memory_mem_ck_n                     : out   std_logic;                                        -- mem_ck_n
            memory_mem_cke                      : out   std_logic;                                        -- mem_cke
            memory_mem_cs_n                     : out   std_logic;                                        -- mem_cs_n
            memory_mem_ras_n                    : out   std_logic;                                        -- mem_ras_n
            memory_mem_cas_n                    : out   std_logic;                                        -- mem_cas_n
            memory_mem_we_n                     : out   std_logic;                                        -- mem_we_n
            memory_mem_reset_n                  : out   std_logic;                                        -- mem_reset_n
            memory_mem_dq                       : inout std_logic_vector(31 downto 0) := (others => 'X'); -- mem_dq
            memory_mem_dqs                      : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs
            memory_mem_dqs_n                    : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs_n
            memory_mem_odt                      : out   std_logic;                                        -- mem_odt
            memory_mem_dm                       : out   std_logic_vector(3 downto 0);                     -- mem_dm
            memory_oct_rzqin                    : in    std_logic                     := 'X';             -- oct_rzqin
            reset_reset_n                       : in    std_logic                     := 'X';             -- reset_n
            mult_data_0_mult_data_m_in1         : out   std_logic_vector(31 downto 0);                    -- m_in1
            mult_data_0_mult_data_m_in2         : out   std_logic_vector(31 downto 0);                    -- m_in2
            mult_data_0_mult_data_m_result      : in    std_logic_vector(31 downto 0) := (others => 'X'); -- m_result
            mult_control_0_mult_control_m_start : out   std_logic_vector(31 downto 0);                    -- m_start
            mult_control_0_mult_control_m_reset : out   std_logic_vector(31 downto 0);                    -- m_reset
            mult_control_0_mult_control_m_done  : in    std_logic_vector(31 downto 0) := (others => 'X'); -- m_done
				div_control_0_div_control_d_start   : out   std_logic_vector(31 downto 0);                    -- d_start
            div_control_0_div_control_d_reset   : out   std_logic_vector(31 downto 0);                    -- d_reset
            div_control_0_div_control_d_done    : in    std_logic_vector(31 downto 0) := (others => 'X'); -- d_done
            div_data_0_div_data_d_denom         : out   std_logic_vector(31 downto 0);                    -- d_denom
            div_data_0_div_data_d_numer         : out   std_logic_vector(31 downto 0);                    -- d_numer
            div_data_0_div_data_d_quot          : in    std_logic_vector(31 downto 0) := (others => 'X'); -- d_quot
            div_data_0_div_data_d_rem           : in    std_logic_vector(31 downto 0) := (others => 'X')  -- d_rem
        );
    end component soc_system;

	component mult_unit is
		PORT( clk, reset, enable		: IN STD_LOGIC;
				mult_a, mult_b					: IN STD_LOGIC_VECTOR(15 DOWNTO 0);
				mult_done						: OUT STD_LOGIC;
				mult_result						: OUT STD_LOGIC_VECTOR(31 DOWNTO 0):= (others => '0')
		    );
	end component mult_unit;
	
	component div_unit is
		PORT(
			clk, reset, enable	: IN STD_LOGIC;
			div_denom, div_numer	: IN STD_LOGIC_VECTOR(15 DOWNTO 0);
			div_done					: OUT STD_LOGIC;
			div_quot, div_rem		: OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
			 );
	end component div_unit;

	--SIGNALS instantiated here
	SIGNAL reset_reset_n : STD_LOGIC;
	
	SIGNAL mult_in1, mult_in2, mult_output_result  	: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL mult_input_start, mult_input_reset 		: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL mult_done 											: STD_LOGIC;
	
	SIGNAL div_in_d, div_in_n, div_out_q, div_out_r : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL div_input_start, div_input_reset 			: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL div_done 											: STD_LOGIC;
	
	
	
	BEGIN

	--port map soc_system here
	    u0 : component soc_system
        port map (
            clk_clk => CLOCK_50,
				reset_reset_n => '1',memory_mem_a => HPS_DDR3_ADDR,
				memory_mem_ba => HPS_DDR3_BA,
				memory_mem_ck => HPS_DDR3_CK_P,
				memory_mem_ck_n => HPS_DDR3_CK_N,
				memory_mem_cke => HPS_DDR3_CKE,
				memory_mem_cs_n => HPS_DDR3_CS_N,
				memory_mem_ras_n => HPS_DDR3_RAS_N,
				memory_mem_cas_n => HPS_DDR3_CAS_N,
				memory_mem_we_n => HPS_DDR3_WE_N,
				memory_mem_reset_n => HPS_DDR3_RESET_N,
				memory_mem_dq => HPS_DDR3_DQ,
				memory_mem_dqs => HPS_DDR3_DQS_P,
				memory_mem_dqs_n => HPS_DDR3_DQS_N,
				memory_mem_odt => HPS_DDR3_ODT,
				memory_mem_dm => HPS_DDR3_DM,
				memory_oct_rzqin => HPS_DDR3_RZQ,
				hps_io_hps_io_emac1_inst_TX_CLK => HPS_ENET_GTX_CLK,
				hps_io_hps_io_emac1_inst_TXD0 => HPS_ENET_TX_DATA(0),
				hps_io_hps_io_emac1_inst_TXD1 => HPS_ENET_TX_DATA(1),
				hps_io_hps_io_emac1_inst_TXD2 => HPS_ENET_TX_DATA(2),
				hps_io_hps_io_emac1_inst_TXD3 => HPS_ENET_TX_DATA(3),
				hps_io_hps_io_emac1_inst_RXD0 => HPS_ENET_RX_DATA(0),
				hps_io_hps_io_emac1_inst_MDIO => HPS_ENET_MDIO,
				hps_io_hps_io_emac1_inst_MDC => HPS_ENET_MDC,
				hps_io_hps_io_emac1_inst_RX_CTL => HPS_ENET_RX_DV,
				hps_io_hps_io_emac1_inst_TX_CTL => HPS_ENET_TX_EN,
				hps_io_hps_io_emac1_inst_RX_CLK => HPS_ENET_RX_CLK,
				hps_io_hps_io_emac1_inst_RXD1 => HPS_ENET_RX_DATA(1),
				hps_io_hps_io_emac1_inst_RXD2 => HPS_ENET_RX_DATA(2),
				hps_io_hps_io_emac1_inst_RXD3 => HPS_ENET_RX_DATA(3),
				hps_io_hps_io_sdio_inst_CMD => HPS_SD_CMD,
				hps_io_hps_io_sdio_inst_D0 => HPS_SD_DATA(0),
				hps_io_hps_io_sdio_inst_D1 => HPS_SD_DATA(1),
				hps_io_hps_io_sdio_inst_CLK => HPS_SD_CLK,
				hps_io_hps_io_sdio_inst_D2 => HPS_SD_DATA(2),
				hps_io_hps_io_sdio_inst_D3 => HPS_SD_DATA(3),
				hps_io_hps_io_usb1_inst_D0 => HPS_USB_DATA(0),
				hps_io_hps_io_usb1_inst_D1 => HPS_USB_DATA(1),
				hps_io_hps_io_usb1_inst_D2 => HPS_USB_DATA(2),
				hps_io_hps_io_usb1_inst_D3 => HPS_USB_DATA(3),
				hps_io_hps_io_usb1_inst_D4 => HPS_USB_DATA(4),
				hps_io_hps_io_usb1_inst_D5 => HPS_USB_DATA(5),
				hps_io_hps_io_usb1_inst_D6 => HPS_USB_DATA(6),
				hps_io_hps_io_usb1_inst_D7 => HPS_USB_DATA(7),
				hps_io_hps_io_usb1_inst_CLK => HPS_USB_CLKOUT,
				hps_io_hps_io_usb1_inst_STP => HPS_USB_STP,
				hps_io_hps_io_usb1_inst_DIR => HPS_USB_DIR,
				hps_io_hps_io_usb1_inst_NXT => HPS_USB_NXT,
				hps_0_h2f_reset_reset_n => reset_reset_n,
            mult_data_0_mult_data_m_in1         => mult_in1,         --       mult_data_0_mult_data.m_in1
            mult_data_0_mult_data_m_in2         => mult_in2,         --                            .m_in2
            mult_data_0_mult_data_m_result      => mult_output_result,      --                            .m_result
            mult_control_0_mult_control_m_start => mult_input_start, -- mult_control_0_mult_control.m_start
            mult_control_0_mult_control_m_reset => mult_input_reset, --                            .m_reset
            mult_control_0_mult_control_m_done  => "0000000000000000000000000000000"  & mult_done, --                            .m_done
	         div_control_0_div_control_d_start   => div_input_start,   --   div_control_0_div_control.d_start
            div_control_0_div_control_d_reset   => div_input_reset,   --                            .d_reset
            div_control_0_div_control_d_done    => "0000000000000000000000000000000"  & div_done,    --                            .d_done
            div_data_0_div_data_d_denom         => div_in_d,         --         div_data_0_div_data.d_denom
            div_data_0_div_data_d_numer         => div_in_n,         --                            .d_numer
            div_data_0_div_data_d_quot          => div_out_q,          --                            .d_quot
            div_data_0_div_data_d_rem           => div_out_r            --                            .d_rem
        );
	
		m0 : component mult_unit
			port map (
				clk => CLOCK_50,
				reset => mult_input_reset(0),
				enable => mult_input_start(0),
				mult_a => mult_in1(15 DOWNTO 0),
				mult_b => mult_in2(15 DOWNTO 0),
				mult_done => mult_done,
				mult_result => mult_output_result
			);
		
		d0 : component div_unit
			port map (
				clk => CLOCK_50,
				reset => div_input_reset(0),
				enable => div_input_start(0),
				div_denom => div_in_d(15 DOWNTO 0),
				div_numer => div_in_n(15 DOWNTO 0),
				div_done => div_done,
				div_quot => div_out_q(15 DOWNTO 0),
				div_rem => div_out_r(15 DOWNTO 0)
			);

End Behaviour;
