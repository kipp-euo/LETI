package edu.java.lab2;

import java.awt.BorderLayout;
import java.awt.FileDialog;
import java.awt.FlowLayout;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.table.DefaultTableModel;

/**
 * UI for Post Office Worker application with exception handling
 * and saving/loading of table data to/from a text file.
 * 
 * @author Beznogov A.V.
 * @version 3.0
 */
public class PostWorker {
	/** Text shown in the search field by default. */
	private static final String SEARCH_PLACEHOLDER = "Поиск по ФИО или издан.";
	/** Separator of cell values in a line of the data file. */
	private static final String SEPARATOR = ";";
	/** Number of columns in the data table. */
	private static final int COLUMN_COUNT = 5;

	private JFrame mainFrame;
	private JToolBar toolBar;
	private JButton openBtn;
	private JButton saveBtn;
	private JButton addBtn;
	private JButton editBtn;
	private JButton deleteBtn;
	private JTable dataTable;
	private DefaultTableModel tableModel;
	private JScrollPane scrollPane;
	private JPanel filterPanel;
	private JComboBox<String> viewCombo;
	private JTextField searchField;
	private JButton searchBtn;

	/**
	 * Exception thrown when the user doesn't specify a name for a search
	 */
	private class EmptySearchNameException extends Exception {
		public EmptySearchNameException() {
			super("Error: No name specified for a search");
		}
	}

	/**
	 * Exception thrown when the user doesn't select a row
	 * before attempting to delete one
	 */
	private class NoRowSelectedException extends Exception {
		public NoRowSelectedException() {
			super("Error: No row selected for deletion");
		}
	}

	/**
	 * Checks if the name for a search is valid.
	 * 
	 * @param bName The search field
	 * @throws EmptySearchNameException No name was specified
	 */
	private void checkSearchName(JTextField bName) throws EmptySearchNameException {
		String sName = bName.getText().trim();
		if (sName.isEmpty() || sName.equals(SEARCH_PLACEHOLDER)) {
			throw new EmptySearchNameException();
		}
	}

	/**
	 * Checks if a row was selected before deletion
	 * 
	 * @param table The data table
	 * @throws NoRowSelectedException No row was selected
	 */
	private void checkSelectedRow(JTable table) throws NoRowSelectedException {
		if (table.getSelectedRow() == -1) {
			throw new NoRowSelectedException();
		}
	}

	/**
	 * Shows a file chooser dialog and returns the full name of the chosen file.
	 * 
	 * @param title Title of the dialog window
	 * @param mode  FileDialog.LOAD or FileDialog.SAVE
	 * @return Full file name, or null if the user cancelled the dialog
	 */
	private String chooseFile(String title, int mode) {
		FileDialog dialog = new FileDialog(mainFrame, title, mode);
		dialog.setFile("*.txt");
		dialog.setVisible(true); // Ask the user for a file
		if (dialog.getFile() == null) {
			return null; // The user pressed "cancel"
		}
		return dialog.getDirectory() + dialog.getFile();
	}

	/**
	 * Saves the content of the data table to a text file.
	 * Every table row is written as one line, cell values are separated by ';'.
	 * Shows an error message if an I/O error occurs.
	 */
	private void saveToFile() {
		String fileName = chooseFile("Сохранение данных", FileDialog.SAVE);
		if (fileName == null) {
			return;
		}
		try (BufferedWriter writer = Files.newBufferedWriter(Paths.get(fileName), StandardCharsets.UTF_8)) {
			for (int i = 0; i < tableModel.getRowCount(); i++) { // For all rows
				StringBuilder line = new StringBuilder();
				for (int j = 0; j < tableModel.getColumnCount(); j++) { // For all columns
					Object value = tableModel.getValueAt(i, j);
					String cell = (value == null) ? "" : value.toString();
					if (j > 0) {
						line.append(SEPARATOR);
					}
					// The separator must not appear inside a cell
					line.append(cell.replace(SEPARATOR, ","));
				}
				writer.write(line.toString());
				writer.newLine();
			}
		} catch (IOException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, "Не удалось сохранить файл:\n" + e.getMessage(),
					"Ошибка сохранения", JOptionPane.ERROR_MESSAGE);
		}
	}

	/**
	 * Loads the content of the data table from a text file.
	 * The table is cleared before loading. Lines with a wrong number of fields are
	 * skipped.
	 * Shows an error message if an I/O error occurs.
	 */
	private void loadFromFile() {
		String fileName = chooseFile("Загрузка данных", FileDialog.LOAD);
		if (fileName == null) {
			return;
		}
		int skipped = 0;
		try (BufferedReader reader = Files.newBufferedReader(Paths.get(fileName), StandardCharsets.UTF_8)) {
			tableModel.setRowCount(0); // Clear the table
			String line;
			while ((line = reader.readLine()) != null) {
				if (line.trim().isEmpty()) {
					continue;
				}
				String[] fields = line.split(SEPARATOR, -1);
				if (fields.length != COLUMN_COUNT) {
					skipped++;
					continue;
				}
				tableModel.addRow(fields); // Write a row into the table
			}
		} catch (IOException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, "Не удалось прочитать файл:\n" + e.getMessage(),
					"Ошибка загрузки", JOptionPane.ERROR_MESSAGE);
			return;
		}
		if (skipped > 0) {
			JOptionPane.showMessageDialog(mainFrame, "Пропущено строк с неверным форматом: " + skipped,
					"Предупреждение", JOptionPane.WARNING_MESSAGE);
		}
	}

	/**
	 * Creates the toolbar for the UI application.
	 * Implements functionality for the Open file, Save file, Add row (+)
	 * and Delete row (trash bin) buttons with exception handling.
	 */
	private void create_toolbar() {
		toolBar = new JToolBar("Панель управления");

		openBtn = new JButton(new ImageIcon("./img/open.png"));
		saveBtn = new JButton(new ImageIcon("./img/save.png"));
		addBtn = new JButton(new ImageIcon("./img/add.png"));
		editBtn = new JButton(new ImageIcon("./img/edit.png"));
		deleteBtn = new JButton(new ImageIcon("./img/delete.png"));

		openBtn.setToolTipText("Открыть файл данных");
		saveBtn.setToolTipText("Сохранить изменения");
		addBtn.setToolTipText("Добавить подписку/клиента");
		editBtn.setToolTipText("Редактировать выбранную запись");
		deleteBtn.setToolTipText("Удалить выбранную запись");

		openBtn.addActionListener(e -> loadFromFile());
		saveBtn.addActionListener(e -> saveToFile());

		addBtn.addActionListener(e -> {
			String id = String.format("%04d", tableModel.getRowCount() + 1);
			tableModel.addRow(new Object[] { id, "", "", "", "" });
		});

		deleteBtn.addActionListener(e -> {
			try {
				checkSelectedRow(dataTable);
				tableModel.removeRow(dataTable.getSelectedRow());
			} catch (NoRowSelectedException ex) {
				JOptionPane.showMessageDialog(mainFrame, ex.getMessage(),
						"Ошибка удаления", JOptionPane.WARNING_MESSAGE);
			}
		});

		toolBar.add(openBtn);
		toolBar.add(saveBtn);
		toolBar.add(addBtn);
		toolBar.add(editBtn);
		toolBar.add(deleteBtn);

		mainFrame.add(toolBar, BorderLayout.NORTH);
	}

	/**
	 * Creates the data table with initial content.
	 */
	private void create_table() {
		String[] columns = { "ID", "ФИО Клиента", "Адрес доставки", "Издание", "Почтальон" };
		Object[][] data = {
				{ "0001", "Иванов И. И.", "Невский пр., д. 12, кв. 4", "Газета 'СПб ведомости'",
						"Смирнов А. В." },
				{ "0002", "Петров П. С.", "ул. Пушкина, д. 5, кв. 18", "Журнал 'Наука и жизнь'",
						"Васильева Е. Н." },
				{ "0003", "Сидорова А. М.", "Садовая ул., д. 28, кв. 2", "Газета 'АиФ'",
						"Смирнов А. В." },
				{ "0004", "Кузнецов Д. В.", "пер. Гривцова, д. 3, кв. 11", "Журнал 'Вокруг света'",
						"Николаев К. М." },
				{ "0005", "Михайлова О. И.", "ул. Марата, д. 45, кв. 9", "Газета 'Коммерсантъ'",
						"Васильева Е. Н." }
		};

		tableModel = new DefaultTableModel(data, columns);
		dataTable = new JTable(tableModel);
		scrollPane = new JScrollPane(dataTable);
		mainFrame.add(scrollPane, BorderLayout.CENTER);
	}

	/**
	 * Creates the bottom panel for the UI application.
	 * Implements searching clients by name with exception handling for empty search
	 * queries.
	 */
	private void create_bottom_panel() {
		filterPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));

		viewCombo = new JComboBox<>(new String[] {
				"Подписки клиентов",
				"Номенклатура газет и журналов",
				"Список почтальонов",
				"Отчет о доставке"
		});
		filterPanel.add(viewCombo);

		searchField = new JTextField(SEARCH_PLACEHOLDER, 22);
		searchBtn = new JButton("Найти");

		searchBtn.addActionListener(l -> {
			try {
				checkSearchName(searchField);
				Vector<String> found_ids = new Vector<>();
				String query_name = searchField.getText().trim();
				for (int i = 0; i < tableModel.getRowCount(); i++) {
					String name = (String) tableModel.getValueAt(i, 1);
					if (name != null && name.contains(query_name)) {
						found_ids.add((String) tableModel.getValueAt(i, 0));
					}
				}
				JOptionPane.showMessageDialog(mainFrame,
						"ID клиентов с подходящим ФИО:\n" + String.join(", ", found_ids));
			} catch (EmptySearchNameException ex) {
				JOptionPane.showMessageDialog(mainFrame, ex.getMessage(),
						"Ошибка поиска", JOptionPane.WARNING_MESSAGE);
			}
		});

		filterPanel.add(searchField);
		filterPanel.add(searchBtn);

		mainFrame.add(filterPanel, BorderLayout.SOUTH);
	}

	/**
	 * Builds and shows the main window of the application.
	 */
	public void show() {
		mainFrame = new JFrame("Работник почты");
		mainFrame.setSize(850, 500);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		mainFrame.setLayout(new BorderLayout());

		create_toolbar();
		create_table();
		create_bottom_panel();

		mainFrame.setVisible(true);
	}

	public static void main(String[] args) {
		new PostWorker().show();
	}
}
