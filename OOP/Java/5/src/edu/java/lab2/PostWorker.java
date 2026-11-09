package edu.java.lab2;

import java.awt.BorderLayout;
import java.awt.FileDialog;
import java.awt.FlowLayout;
import java.io.File;
import java.io.IOException;
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
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * UI for Post Office Worker application with exception handling
 * and saving/loading of table data to/from an XML file (DOM).
 * 
 * @author Beznogov A.V.
 * @version 4.0
 */
public class PostWorker {
	/** Text shown in the search field by default. */
	private static final String SEARCH_PLACEHOLDER = "Поиск по ФИО или издан.";
	/** Name of the root element of the XML document. */
	private static final String ROOT_TAG = "subscriptions";
	/** Name of the element describing one table row. */
	private static final String ROW_TAG = "subscription";
	/** Attribute names of the row element, in the order of table columns. */
	private static final String[] ATTRS = { "id", "client", "address", "publication", "postman" };

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
	 * Exception thrown when an XML file is well-formed,
	 * but has an unexpected structure (wrong root element).
	 */
	private class InvalidXmlStructureException extends Exception {
		public InvalidXmlStructureException(String rootName) {
			super("Неверная структура XML: корневой элемент <" + rootName
					+ ">, ожидался <" + ROOT_TAG + ">");
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
		dialog.setFile("*.xml");
		dialog.setVisible(true); // Ask the user for a file
		if (dialog.getFile() == null) {
			return null; // The user pressed "cancel"
		}
		return dialog.getDirectory() + dialog.getFile();
	}

	/**
	 * Creates a DOM parser. External entities and DTDs are forbidden,
	 * so a foreign XML file cannot make the parser read other files.
	 * 
	 * @return New document builder
	 * @throws ParserConfigurationException The parser cannot be created
	 */
	private DocumentBuilder createBuilder() throws ParserConfigurationException {
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
		return factory.newDocumentBuilder();
	}

	/**
	 * Saves the content of the data table to an XML file.
	 * The root element is &lt;subscriptions&gt;, every table row becomes
	 * a &lt;subscription&gt; element whose attributes hold the cell values.
	 * Shows an error message if the document cannot be built or written.
	 */
	private void saveToFile() {
		String fileName = chooseFile("Сохранение данных", FileDialog.SAVE);
		if (fileName == null) {
			return;
		}
		if (dataTable.isEditing()) {
			dataTable.getCellEditor().stopCellEditing(); // Don't lose the cell being edited
		}
		try {
			Document doc = createBuilder().newDocument(); // Empty document

			Element root = doc.createElement(ROOT_TAG); // Root element
			doc.appendChild(root);

			for (int i = 0; i < tableModel.getRowCount(); i++) { // For all rows
				Element row = doc.createElement(ROW_TAG);
				root.appendChild(row);
				for (int j = 0; j < ATTRS.length; j++) { // For all columns
					Object value = tableModel.getValueAt(i, j);
					row.setAttribute(ATTRS[j], value == null ? "" : value.toString());
				}
			}

			Transformer trans = TransformerFactory.newInstance().newTransformer();
			trans.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
			trans.setOutputProperty(OutputKeys.INDENT, "yes");
			trans.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
			// StreamResult(File) writes bytes itself, so the UTF-8 encoding is respected
			trans.transform(new DOMSource(doc), new StreamResult(new File(fileName)));
		} catch (ParserConfigurationException | TransformerException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, "Не удалось сформировать XML:\n" + e.getMessage(),
					"Ошибка сохранения", JOptionPane.ERROR_MESSAGE);
		}
	}

	/**
	 * Loads the content of the data table from an XML file.
	 * The document is parsed and checked first, and only then the table is
	 * cleared, so a broken file does not destroy the current data.
	 * Shows an error message if the file is missing, not well-formed
	 * or has a wrong structure.
	 */
	private void loadFromFile() {
		String fileName = chooseFile("Загрузка данных", FileDialog.LOAD);
		if (fileName == null) {
			return;
		}
		try {
			Document doc = createBuilder().parse(new File(fileName)); // Read the document
			doc.getDocumentElement().normalize(); // Normalize the tree

			Element root = doc.getDocumentElement();
			if (!ROOT_TAG.equals(root.getTagName())) {
				throw new InvalidXmlStructureException(root.getTagName());
			}

			NodeList rows = root.getElementsByTagName(ROW_TAG);
			tableModel.setRowCount(0); // Clear the table
			for (int i = 0; i < rows.getLength(); i++) {
				Node node = rows.item(i);
				if (node.getNodeType() != Node.ELEMENT_NODE) {
					continue;
				}
				Element row = (Element) node;
				String[] cells = new String[ATTRS.length];
				for (int j = 0; j < ATTRS.length; j++) {
					cells[j] = row.getAttribute(ATTRS[j]); // "" if the attribute is missing
				}
				tableModel.addRow(cells); // Write a row into the table
			}
		} catch (SAXException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, "Файл не является корректным XML:\n" + e.getMessage(),
					"Ошибка загрузки", JOptionPane.ERROR_MESSAGE);
		} catch (IOException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, "Не удалось прочитать файл:\n" + e.getMessage(),
					"Ошибка загрузки", JOptionPane.ERROR_MESSAGE);
		} catch (ParserConfigurationException | InvalidXmlStructureException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(mainFrame, e.getMessage(),
					"Ошибка загрузки", JOptionPane.ERROR_MESSAGE);
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

		openBtn.setToolTipText("Открыть XML-файл");
		saveBtn.setToolTipText("Сохранить в XML-файл");
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
