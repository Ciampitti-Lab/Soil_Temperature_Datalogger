# Libraries ----
library(plotly)
source("back-end.R")

# Page ----
main_page <- function(data){
  
  fluidPage(
    
    ## Header ----
    div(
      class = "header",
      tags$h2(
        "Temperature sensors hub"
      ),
      div(
        class = "download-button",
        tags$h3(
          "Download the data",
        ),
        downloadButton(
          outputId = "downloadData",
          label = "Download",
          icon = icon("download"),
          style = "background-color: rgb(139, 148, 165);"
        )
      )
    ),
    
    br(),
    
    ## Last readings by block (cards) ----
    
    fluidRow(
      uiOutput(outputId = "lastReadingCard"),
    ),
    
    ## Last readings by block (graph) ----
    
    div(
      class = "block",
      tags$h3(
        "Last readings by block"
      ),
      hr(),
      fluidRow(
        class = "inside-block",
        column(
          width = 8,
          class = "block-graph",
          radioGroupButtons(
            inputId = "lastReadingRadio",
            label = "",
            status = "primary",
            direction = "horizontal",
            choices = create_block_options(data),
          ),
          br(),
          plotlyOutput(outputId = "lastReadingGraph"),
        ),
        column(
          width = 4,
          uiOutput(outputId = "maxCard"),
          uiOutput(outputId = "minCard")
        )
      )
    ),
    
    br(),
    
    div(
      style = "background-color: rgb(104, 115, 135); border-radius: 10px; color: white; padding: 2vh",
      tags$h3(
        "Temperature curves",
        style = "text-align: center",
      ),
      div(
        style = "background-color: rgb(139, 148, 165); color: white; padding: 2vh; margin-bottom: 0.5vh",
        tags$h4(
          "By block"
        ),
        div(
          style = "background-color: rgb(136, 171, 184); padding: 0.5vh; color: black; text-align: center; width: 50%; margin: auto",
          radioButtons(
            inputId = "tempCurvesBlockRadio",
            label = "",
            inline = TRUE,
            choices = create_block_options(data),
          )
        ),
        br(),
        plotlyOutput(outputId = "tempCurveBlock"),
        br(),
        div(
          style = "background-color: rgb(136, 171, 184); padding: 0.5vh; color: black; text-align: center; width: 25%; margin: auto; font-size: 16px",
          radioButtons(
            inputId = "tempCurvesTimeRadio",
            label = "",
            inline = TRUE,
            choices = c("Hour" = "hour",
                        "Day" = "day",
                        "Week" = "week",
                        "Month" = "month"),
          )
        ),
      ),
      div(
        style = "background-color: rgb(139, 148, 165); color: white; padding: 2vh; margin-top: 0.5vh",
        tags$h4(
          "Mean of blocks"
        ),
        br(),
        plotlyOutput(outputId = "tempCurveMeanBlock"),
        br(),
        div(
          style = "background-color: rgb(136, 171, 184); padding: 0.5vh; color: black; text-align: center; width: 25%; margin: auto; font-size: 16px",
          radioButtons(
            inputId = "tempCurvesMeanTimeRadio",
            label = "",
            inline = TRUE,
            choices = c("Hour" = "hour",
                        "Day" = "day",
                        "Week" = "week",
                        "Month" = "month"),
          )
        ),
      )
    ),
    
    br(),
    
    div(
      style = "display: inline-block; text-align: center",
      tags$h5(
        "This tool was developed by the Ciampitti Lab Group"
      )
    )
    
  )
  
}

# Small functions ----
create_block_options <- function(data){
  choices <- unique(data$Group)
  names <- paste("Block", as.character(choices))
  named_list <- as.list(setNames(choices, names))
  return(named_list)
}

