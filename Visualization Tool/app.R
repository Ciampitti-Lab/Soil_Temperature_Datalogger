# Libraries ----
library(shiny)
library(shinyWidgets)
library(plotly)
source("back-end.R")
source("front-end.R")

# Data ----
data <- obtaining_data(file.source = "csv", file.path = "file.csv")

# UI ----
ui <- tagList(
  
  ## Include CSS ----
  includeCSS(path = "www/styles.css"),
  
  ## Front-End ----
  main_page(data)
  
)

# Server ----
server <- function(input, output, session) {
  
  output$lastReadingGraph <- renderPlotly({
    last_reading_graph(data = data,
                       group.selected = input$lastReadingRadio)
  })
  
  output$lastReadingCard <- renderUI({
    cards_last(data = data)
  })
  
  output$tempCurveBlock <- renderPlotly(
    curve_by_blocks(database = data,
                    group.selected = input$tempCurvesBlockRadio,
                    time.scale = input$tempCurvesTimeRadio)
  )
  
  output$maxCard <- renderUI({
    cards_max_min(data = data,
                  block = input$tempCurvesBlockRadio,
                  max.min = "max")
  })
  
  output$minCard <- renderUI({
    cards_max_min(data = data,
                  block = input$tempCurvesBlockRadio,
                  max.min = "min")
  })
  
  output$tempCurveMeanBlock <- renderPlotly(
    curve_mean_blocks(database = data,
                      time.scale = input$tempCurvesMeanTimeRadio)
  )
  
  output$downloadData <- downloadHandler(
    filename = function(){
      paste0("dataVisualizationHub", Sys.Date(),".csv")
    },
    content = function(file){
      write.csv(data, file)
    }
  )
  
}

# App ----
shinyApp(ui, server)