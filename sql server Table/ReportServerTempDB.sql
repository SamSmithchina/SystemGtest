USE [ReportServerTempDB]
GO
/****** Object:  Table [dbo].[TempCatalog]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[TempCatalog](
	[EditSessionID] [varchar](32) NOT NULL,
	[TempCatalogID] [uniqueidentifier] NOT NULL,
	[ContextPath] [nvarchar](425) NOT NULL,
	[Name] [nvarchar](425) NOT NULL,
	[Content] [varbinary](max) NULL,
	[Description] [nvarchar](max) NULL,
	[Intermediate] [uniqueidentifier] NULL,
	[IntermediateIsPermanent] [bit] NOT NULL,
	[Property] [nvarchar](max) NULL,
	[Parameter] [nvarchar](max) NULL,
	[OwnerID] [uniqueidentifier] NOT NULL,
	[CreationTime] [datetime] NOT NULL,
	[ExpirationTime] [datetime] NOT NULL,
	[DataCacheHash] [varbinary](64) NULL,
 CONSTRAINT [PK_TempCatalog] PRIMARY KEY CLUSTERED 
(
	[EditSessionID] ASC,
	[ContextPath] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
 CONSTRAINT [UNIQ_TempCatalogID] UNIQUE NONCLUSTERED 
(
	[TempCatalogID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[SnapshotData]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SnapshotData](
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[CreatedDate] [datetime] NOT NULL,
	[ParamsHash] [int] NULL,
	[QueryParams] [ntext] NULL,
	[EffectiveParams] [ntext] NULL,
	[Description] [nvarchar](512) NULL,
	[DependsOnUser] [bit] NULL,
	[PermanentRefcount] [int] NOT NULL,
	[TransientRefcount] [int] NOT NULL,
	[ExpirationDate] [datetime] NOT NULL,
	[PageCount] [int] NULL,
	[HasDocMap] [bit] NULL,
	[Machine] [nvarchar](512) NOT NULL,
	[PaginationMode] [smallint] NULL,
	[ProcessingFlags] [int] NULL,
	[IsCached] [bit] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SessionLock]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[SessionLock](
	[SessionID] [varchar](32) NOT NULL,
	[LockVersion] [int] NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[SessionData]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[SessionData](
	[SessionID] [varchar](32) NOT NULL,
	[CompiledDefinition] [uniqueidentifier] NULL,
	[SnapshotDataID] [uniqueidentifier] NULL,
	[IsPermanentSnapshot] [bit] NULL,
	[ReportPath] [nvarchar](464) NULL,
	[Timeout] [int] NOT NULL,
	[AutoRefreshSeconds] [int] NULL,
	[Expiration] [datetime] NOT NULL,
	[ShowHideInfo] [image] NULL,
	[DataSourceInfo] [image] NULL,
	[OwnerID] [uniqueidentifier] NOT NULL,
	[EffectiveParams] [ntext] NULL,
	[CreationTime] [datetime] NOT NULL,
	[HasInteractivity] [bit] NULL,
	[SnapshotExpirationDate] [datetime] NULL,
	[HistoryDate] [datetime] NULL,
	[PageHeight] [float] NULL,
	[PageWidth] [float] NULL,
	[TopMargin] [float] NULL,
	[BottomMargin] [float] NULL,
	[LeftMargin] [float] NULL,
	[RightMargin] [float] NULL,
	[AwaitingFirstExecution] [bit] NULL,
	[EditSessionID] [varchar](32) NULL,
	[DataSetInfo] [varbinary](max) NULL,
	[SitePath] [nvarchar](440) NULL,
	[SiteZone] [int] NOT NULL,
	[ReportDefinitionPath] [nvarchar](464) NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[SegmentedChunk]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SegmentedChunk](
	[ChunkId] [uniqueidentifier] NOT NULL,
	[SnapshotDataId] [uniqueidentifier] NOT NULL,
	[ChunkFlags] [tinyint] NOT NULL,
	[ChunkName] [nvarchar](260) NOT NULL,
	[ChunkType] [int] NOT NULL,
	[Version] [smallint] NOT NULL,
	[MimeType] [nvarchar](260) NULL,
	[Machine] [nvarchar](512) NOT NULL,
	[SegmentedChunkId] [bigint] IDENTITY(1,1) NOT NULL,
 CONSTRAINT [PK_SegmentedChunk] PRIMARY KEY CLUSTERED 
(
	[SegmentedChunkId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Segment]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Segment](
	[SegmentId] [uniqueidentifier] NOT NULL,
	[Content] [varbinary](max) NULL,
 CONSTRAINT [PK_Segment] PRIMARY KEY CLUSTERED 
(
	[SegmentId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[PersistedStream]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[PersistedStream](
	[SessionID] [varchar](32) NOT NULL,
	[Index] [int] NOT NULL,
	[Content] [image] NULL,
	[Name] [nvarchar](260) NULL,
	[MimeType] [nvarchar](260) NULL,
	[Extension] [nvarchar](260) NULL,
	[Encoding] [nvarchar](260) NULL,
	[Error] [nvarchar](512) NULL,
	[RefCount] [int] NOT NULL,
	[ExpirationDate] [datetime] NOT NULL,
 CONSTRAINT [PK_PersistedStream] PRIMARY KEY CLUSTERED 
(
	[SessionID] ASC,
	[Index] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ExecutionCache]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ExecutionCache](
	[ExecutionCacheID] [uniqueidentifier] NOT NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[ExpirationFlags] [int] NOT NULL,
	[AbsoluteExpiration] [datetime] NULL,
	[RelativeExpiration] [int] NULL,
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[LastUsedTime] [datetime] NOT NULL,
	[ParamsHash] [int] NOT NULL,
 CONSTRAINT [PK_ExecutionCache] PRIMARY KEY NONCLUSTERED 
(
	[ExecutionCacheID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[DBUpgradeHistory]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DBUpgradeHistory](
	[UpgradeID] [bigint] IDENTITY(1,1) NOT NULL,
	[DbVersion] [nvarchar](25) NULL,
	[User] [nvarchar](128) NULL,
	[DateTime] [datetime] NULL,
 CONSTRAINT [PK_DBUpgradeHistory] PRIMARY KEY CLUSTERED 
(
	[UpgradeID] DESC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ChunkSegmentMapping]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ChunkSegmentMapping](
	[ChunkId] [uniqueidentifier] NOT NULL,
	[SegmentId] [uniqueidentifier] NOT NULL,
	[StartByte] [bigint] NOT NULL,
	[LogicalByteCount] [int] NOT NULL,
	[ActualByteCount] [int] NOT NULL,
 CONSTRAINT [PK_ChunkSegmentMapping] PRIMARY KEY CLUSTERED 
(
	[ChunkId] ASC,
	[SegmentId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ChunkData]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ChunkData](
	[ChunkID] [uniqueidentifier] NOT NULL,
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[ChunkFlags] [tinyint] NULL,
	[ChunkName] [nvarchar](260) NULL,
	[ChunkType] [int] NULL,
	[Version] [smallint] NULL,
	[MimeType] [nvarchar](260) NULL,
	[Content] [image] NULL,
 CONSTRAINT [PK_ChunkData] PRIMARY KEY NONCLUSTERED 
(
	[ChunkID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[TempDataSources]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[TempDataSources](
	[DSID] [uniqueidentifier] NOT NULL,
	[ItemID] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](260) NULL,
	[Extension] [nvarchar](260) NULL,
	[Link] [uniqueidentifier] NULL,
	[CredentialRetrieval] [int] NULL,
	[Prompt] [ntext] NULL,
	[ConnectionString] [image] NULL,
	[OriginalConnectionString] [image] NULL,
	[OriginalConnectStringExpressionBased] [bit] NULL,
	[UserName] [image] NULL,
	[Password] [image] NULL,
	[Flags] [int] NULL,
	[Version] [int] NOT NULL,
 CONSTRAINT [PK_DataSource] PRIMARY KEY CLUSTERED 
(
	[DSID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[TempDataSets]    Script Date: 11/05/2018 11:02:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[TempDataSets](
	[ID] [uniqueidentifier] NOT NULL,
	[ItemID] [uniqueidentifier] NOT NULL,
	[LinkID] [uniqueidentifier] NULL,
	[Name] [nvarchar](260) NOT NULL,
 CONSTRAINT [PK_TempDataSet] PRIMARY KEY NONCLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Default [DF__DBUpgradeH__User__1CF15040]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[DBUpgradeHistory] ADD  DEFAULT (suser_sname()) FOR [User]
GO
/****** Object:  Default [DF__DBUpgrade__DateT__1DE57479]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[DBUpgradeHistory] ADD  DEFAULT (getdate()) FOR [DateTime]
GO
/****** Object:  Default [DF__Execution__LastU__1FCDBCEB]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[ExecutionCache] ADD  DEFAULT (getdate()) FOR [LastUsedTime]
GO
/****** Object:  Default [DF__Execution__Param__2A4B4B5E]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[ExecutionCache] ADD  DEFAULT ((0)) FOR [ParamsHash]
GO
/****** Object:  Default [DF_Segment_SegmentId]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[Segment] ADD  CONSTRAINT [DF_Segment_SegmentId]  DEFAULT (newsequentialid()) FOR [SegmentId]
GO
/****** Object:  Default [DF_SegmentedChunk_ChunkId]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[SegmentedChunk] ADD  CONSTRAINT [DF_SegmentedChunk_ChunkId]  DEFAULT (newsequentialid()) FOR [ChunkId]
GO
/****** Object:  Default [DF__SessionDa__SiteZ__2B3F6F97]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[SessionData] ADD  DEFAULT ((0)) FOR [SiteZone]
GO
/****** Object:  Default [DF__SessionLo__LockV__2D27B809]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[SessionLock] ADD  DEFAULT ((0)) FOR [LockVersion]
GO
/****** Object:  Default [DF__SnapshotD__IsCac__108B795B]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[SnapshotData] ADD  DEFAULT ((0)) FOR [IsCached]
GO
/****** Object:  Default [DF__TempCatal__Inter__239E4DCF]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[TempCatalog] ADD  DEFAULT ((0)) FOR [IntermediateIsPermanent]
GO
/****** Object:  Check [Positive_ActualByteCount]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_ActualByteCount] CHECK  (([ActualByteCount]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_ActualByteCount]
GO
/****** Object:  Check [Positive_LogicalByteCount]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_LogicalByteCount] CHECK  (([LogicalByteCount]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_LogicalByteCount]
GO
/****** Object:  Check [Positive_StartByte]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_StartByte] CHECK  (([StartByte]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_StartByte]
GO
/****** Object:  ForeignKey [FK_DataSetItemID]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[TempDataSets]  WITH NOCHECK ADD  CONSTRAINT [FK_DataSetItemID] FOREIGN KEY([ItemID])
REFERENCES [dbo].[TempCatalog] ([TempCatalogID])
GO
ALTER TABLE [dbo].[TempDataSets] CHECK CONSTRAINT [FK_DataSetItemID]
GO
/****** Object:  ForeignKey [FK_DataSourceItemID]    Script Date: 11/05/2018 11:02:20 ******/
ALTER TABLE [dbo].[TempDataSources]  WITH NOCHECK ADD  CONSTRAINT [FK_DataSourceItemID] FOREIGN KEY([ItemID])
REFERENCES [dbo].[TempCatalog] ([TempCatalogID])
GO
ALTER TABLE [dbo].[TempDataSources] CHECK CONSTRAINT [FK_DataSourceItemID]
GO
